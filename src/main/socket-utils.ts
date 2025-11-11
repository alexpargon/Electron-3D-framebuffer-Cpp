import * as net from "net";

// Extend the Socket type to include fd handling
declare module "net" {
  interface Socket {
    _handle: {
      fd: number;
    };
  }
}

export function getSocketFileDescriptor(socket: net.Socket): number | null {
  try {
    return socket._handle?.fd ?? null;
  } catch {
    return null;
  }
}
