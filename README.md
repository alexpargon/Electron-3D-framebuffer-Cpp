# Electron Framebuffer example

## What will you find?

- C++ app that draws a red cube and shares the framebuffer with the electron APP
- a Electron APP that is able to draw the framebuffer in a React component

## How to get started?

1. Install dependencies (use node >= 20)
2. Cmake . and make the c++ app
3. start the c++ app
4. start the electron app
5. you should see the cube rotating both in the C++ app and in Electron
<img width="1816" height="763" alt="image" src="https://github.com/user-attachments/assets/5888088d-dea1-43c9-9a6b-79c52eccd6f8" />

```bash
cmake . && make && ./spinning_cube
```

2. Start the project

```bash
npm i && npm start
```

## Authors

<table>
  <tbody>
    <td align="center">
      <a href="https://github.com/alexpargon">
        <img src="https://github.com/alexpargon.png?size=100" />
        <p>Alejandro Parcet</p>
      </a>
    </td>
  </tbody>
</table>
