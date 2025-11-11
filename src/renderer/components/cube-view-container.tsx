import React from "react";
import { CubeView } from "./cube-view";
import { PixelShmView } from "./pixel-shm-view";
import { Card } from "./ui/card";

export const CubeViewContainer: React.FC = () => {
  return (
    <div
      style={{
        display: "flex",
        gap: "20px",
        justifyContent: "center",
        padding: "20px",
      }}
    >
      <Card>
        <div className="p-4">
          <h3 className="text-lg font-semibold mb-2">File-based View</h3>
          <CubeView />
        </div>
      </Card>

      <Card>
        <div className="p-4">
          <h3 className="text-lg font-semibold mb-2">
            Pixel (Shared Memory) View
          </h3>
          <PixelShmView />
        </div>
      </Card>
    </div>
  );
};
