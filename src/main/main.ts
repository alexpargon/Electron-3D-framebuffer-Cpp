import * as fs from "fs";
import * as net from "net";
import path from "path";
import { app, BrowserWindow, shell, ipcMain } from "electron";
import log from "electron-log/main";
import { getSocketFileDescriptor } from "./socket-utils";

// Handle creating/removing shortcuts on Windows when installing/uninstalling.
if (require("electron-squirrel-startup")) {
  app.quit();
}

// Setup DMA-BUF Unix Domain Socket Server
const setupDMABufServer = (mainWindow: BrowserWindow) => {
  const socketPath = "/tmp/electron_dmabuf.sock";

  // Remove existing socket file if it exists
  try {
    fs.unlinkSync(socketPath);
  } catch (_) {
    // Ignore error if file doesn't exist
  }

  const server = net.createServer((connection) => {
    connection.on("data", (_data) => {
      // Extract file descriptor using our utility
      const fd = getSocketFileDescriptor(connection);
      if (fd !== null) {
        // Send the DMA-BUF file descriptor to the renderer
        mainWindow.webContents.send("dmabuf-received", { fd });
        log.info("Sent DMA-BUF fd to renderer:", fd);
      }
    });

    connection.on("error", (err) => {
      log.error("DMA-BUF socket connection error:", err);
    });
  });

  server.on("error", (err) => {
    log.error("DMA-BUF socket server error:", err);
  });

  server.listen(socketPath, () => {
    log.info("DMA-BUF socket server listening on", socketPath);
    fs.chmodSync(socketPath, "777"); // Ensure the socket is accessible
  });

  return server;
};

const createWindow = () => {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 1280, // Width to accommodate both 512px views plus padding
    height: 720, // Height for the views plus header and padding
    minHeight: 720,
    minWidth: 1280,
    webPreferences: {
      preload: path.resolve(__dirname, "../../.vite/build/preload.js"),
      contextIsolation: true,
      nodeIntegration: false,
      sandbox: false,
    },
  });

  log.initialize();
  const level = process.env.NODE_ENV === "development" ? "debug" : "silly";
  log.transports.console.format =
    "[{y}-{m}-{d} {h}:{i}:{s}.{ms}] [{level}] {text}";
  log.transports.console.level = level;
  log.transports.file.level = false;
  Object.assign(console, log.functions);

  // Initialize both file-based and DMA-BUF approaches
  const server = setupDMABufServer(mainWindow);

  // Register IPC handlers for both approaches
  ipcMain.handle("get-framebuffer-path", () => "/tmp/cube_framebuffer.bin");

  // Clean up when window is closed
  mainWindow.on("closed", () => {
    server.close();
  });

  mainWindow.webContents.setWindowOpenHandler((details) => {
    shell.openExternal(details.url); // Open URL in user's browser.
    return { action: "deny" }; // Prevent the app from opening the URL.
  });

  // and load the index.html of the app.
  if (MAIN_WINDOW_VITE_DEV_SERVER_URL) {
    mainWindow.loadURL(MAIN_WINDOW_VITE_DEV_SERVER_URL);
  } else {
    mainWindow.loadFile(
      path.join(__dirname, `../renderer/${MAIN_WINDOW_VITE_NAME}/index.html`)
    );
  }

  // Open the DevTools.
  mainWindow.webContents.openDevTools({
    mode: "detach",
  });
};

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.on("ready", createWindow);

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on("window-all-closed", () => {
  if (process.platform !== "darwin") {
    app.quit();
  }
});

app.on("activate", () => {
  // On OS X it's common to re-create a window in the app when the
  // dock icon is clicked and there are no other windows open.
  if (BrowserWindow.getAllWindows().length === 0) {
    createWindow();
  }
});

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and import them here.
