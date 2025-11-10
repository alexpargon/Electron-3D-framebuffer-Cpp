import { readFileSync } from "fs";
import { contextBridge } from "electron";

import log from "electron-log/main";

log.info("[preload] Preload script loaded");

const SHM_PATH = "/dev/shm/cube_framebuffer";

contextBridge.exposeInMainWorld("electronAPI", {
  readFramebuffer: () => {
    try {
      const buffer = readFileSync(SHM_PATH);
      return buffer.buffer;
    } catch {
      return null;
    }
  },
});
