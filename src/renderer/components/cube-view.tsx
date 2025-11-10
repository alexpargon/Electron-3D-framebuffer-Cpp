import React, { useEffect, useRef } from "react";

const WIDTH = 512;
const HEIGHT = 512;
const FRAMEBUFFER_PATH = "/tmp/cube_framebuffer.bin";

async function fetchFramebuffer(): Promise<Uint8Array | null> {
  try {
    // @ts-ignore: Custom API exposed by preload
    const buffer = await window.electronAPI.readFramebuffer(FRAMEBUFFER_PATH);
    return buffer ? new Uint8Array(buffer) : null;
  } catch (e) {
    console.error("Error reading framebuffer:", e);
    return null;
  }
}

function bufferToImageData(buffer: Uint8Array): ImageData {
  // Only process if buffer is the expected size
  if (buffer.length !== WIDTH * HEIGHT * 4) {
    return null;
  }
  // Flip vertically
  const flipped = new Uint8ClampedArray(buffer.length);
  const rowSize = WIDTH * 4;
  for (let y = 0; y < HEIGHT; y++) {
    const srcStart = (HEIGHT - 1 - y) * rowSize;
    const destStart = y * rowSize;
    flipped.set(buffer.slice(srcStart, srcStart + rowSize), destStart);
  }
  return new ImageData(flipped, WIDTH, HEIGHT);
}

export const CubeView: React.FC = () => {
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    let running = true;
    async function update() {
      if (!running) return;
      const buffer = await fetchFramebuffer();
      if (buffer && canvasRef.current) {
        const ctx = canvasRef.current.getContext("2d");
        if (ctx) {
          const imgData = bufferToImageData(buffer);
          if (imgData) {
            ctx.putImageData(imgData, 0, 0);
          }
        }
      }
      setTimeout(update, 16); // ~60 FPS
    }
    update();
    return () => {
      running = false;
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
