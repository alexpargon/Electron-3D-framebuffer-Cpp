import React, { useEffect, useRef } from "react";

const WIDTH = 512;
const HEIGHT = 512;

export const DMABufView: React.FC = () => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    const canvas = canvasRef.current;
    if (!canvas) return;

    const cleanup = window.electronAPI.onDMABufReceived(
      (data: { fd: number }) => {
        if (data && typeof data.fd === "number") {
          const ctx = canvas.getContext("2d");
          if (ctx) {
            // TODO: Here we'll implement the DMA-BUF rendering once the
            // native implementation is complete
            ctx.fillStyle = "#333";
            ctx.fillRect(0, 0, WIDTH, HEIGHT);
            ctx.fillStyle = "#fff";
            ctx.font = "14px Arial";
            ctx.textAlign = "center";
            ctx.fillText(`DMA-BUF FD: ${data.fd}`, WIDTH / 2, HEIGHT / 2);
          }
        }
      }
    );

    return () => {
      if (cleanup) cleanup();
    };
  }, []);

  return (
    <canvas
      ref={canvasRef}
      width={WIDTH}
      height={HEIGHT}
      style={{ border: "1px solid #333" }}
    />
  );
};
