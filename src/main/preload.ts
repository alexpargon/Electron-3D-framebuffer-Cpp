import { readFile } from "fs/promises";
import { contextBridge, ipcRenderer } from "electron";
import log from "electron-log/main";

log.info("[preload] Preload script loaded");

contextBridge.exposeInMainWorld("electronAPI", {
  readFramebuffer: async (path: string) => {
    try {
      const buffer = await readFile(path);
      return buffer.buffer;
    } catch {
      return null;
    }
  },
  // DMA-BUF API
  onDMABufReceived: (callback: (data: { fd: number }) => void) => {
    const wrappedCallback = (_event: any, data: { fd: number }) =>
      callback(data);
    ipcRenderer.on("dmabuf-received", wrappedCallback);
    return () => {
      ipcRenderer.removeListener("dmabuf-received", wrappedCallback);
    };
  },
});
