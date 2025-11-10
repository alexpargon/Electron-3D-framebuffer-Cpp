import { readFile } from "fs/promises";
import { contextBridge } from "electron";

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
