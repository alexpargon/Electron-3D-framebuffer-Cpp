import React from "react";
import Title from "@Renderer/components/title";
import { CubeView } from "@Renderer/components/cube-view";

function Home() {
  return (
    <div>
      <Title>Home</Title>
      <CubeView />
      <p>this is the new home</p>
    </div>
  );
}

export default Home;
