import React from "react";
import Title from "@Renderer/components/title";
import { CubeViewContainer } from "@Renderer/components/cube-view-container";

function Home() {
  return (
    <div>
      <Title>Spinning Cube Demo</Title>
      <CubeViewContainer />
    </div>
  );
}

export default Home;
