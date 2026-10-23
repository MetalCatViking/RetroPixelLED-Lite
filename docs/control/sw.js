const CACHE_NAME = "retropixel-shell-v4";
const APP_SHELL = [
  "index.htm",
  "index_EN.htm",
  "manifest.json",
  "manifest_EN.json",
  "icons/icon-192.png",
  "icons/icon-512.png"
];

self.addEventListener("install", (event) => {
  event.waitUntil(
    caches.open(CACHE_NAME).then((cache) => {
      const shellURLs = APP_SHELL.map((path) => new URL(path, self.registration.scope).href);
      return cache.addAll(shellURLs);
    })
  );
  self.skipWaiting();
});

self.addEventListener("activate", (event) => {
  event.waitUntil(
    caches.keys().then((keys) =>
      Promise.all(keys.filter((key) => key !== CACHE_NAME).map((key) => caches.delete(key)))
    )
  );
  self.clients.claim();
});

self.addEventListener("fetch", (event) => {
  // Never intercept or cache requests to the LED panel (POST /texto, etc.).
  // Only serve the PWA's static app shell from cache.
  if (event.request.method !== "GET") return;

  const requestURL = new URL(event.request.url);
  if (requestURL.origin !== self.location.origin) return;

  const isAppShell = APP_SHELL.some((path) =>
    new URL(path, self.registration.scope).pathname === requestURL.pathname
  );
  if (!isAppShell) return;

  event.respondWith(
    caches.open(CACHE_NAME).then((cache) =>
      cache.match(event.request, { ignoreSearch: true }).then((cached) => cached || fetch(event.request))
    )
  );
});
