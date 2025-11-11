interface ElectronAPI {
  getFramebufferPath: () => Promise<string>;
  readFramebuffer: (path: string) => Promise<ArrayBuffer | null>;
  onDMABufReceived: (callback: (data: { fd: number }) => void) => () => void;
}

declare global {
  interface Window {
    electronAPI: ElectronAPI;
  }
}
