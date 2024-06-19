#!/usr/bin/sh
#!/usr/bin/sh

BROWSER=firefox

source ./build-wasm.sh
python3 -m http.server &
firefox localhost:8000/app.html
