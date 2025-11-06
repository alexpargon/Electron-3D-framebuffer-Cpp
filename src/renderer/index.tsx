import React from "react";
import ReactDOM from "react-dom/client";
import { HashRouter } from "react-router-dom";
import "./index.css";
import App from "./app";
import ErrorBoundary from "./errorBoundary";
import { ThemeProvider } from "./components/theme-provider";

ReactDOM.createRoot(document.getElementById("root")).render(
  <ErrorBoundary>
    <HashRouter>
      <React.StrictMode>
        <ThemeProvider defaultTheme="system" storageKey="vite-ui-theme">
          <App />
        </ThemeProvider>
      </React.StrictMode>
    </HashRouter>
  </ErrorBoundary>
);
