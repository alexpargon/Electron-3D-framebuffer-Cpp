import { readFile } from "fs/promises";
import { contextBridge } from "electron";
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
});
