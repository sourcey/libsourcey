#!/usr/bin/env sh
openssl req -x509 -newkey rsa:2048 -nodes -sha256 -days 365 -subj "/CN=localhost" -addext "subjectAltName=DNS:localhost,IP:127.0.0.1" -keyout key.pem -out cert.pem
