#!/usr/bin/env bash
set -euo pipefail

OUT_FILE="components/ledriver_app_config/factory_nvs.csv"

: "${LEDRIVER_WIFI_SSID:?Missing LEDRIVER_WIFI_SSID}"
: "${LEDRIVER_WIFI_PASSWORD:?Missing LEDRIVER_WIFI_PASSWORD}"
: "${LEDRIVER_UPDATE_BASE_URL:?Missing LEDRIVER_UPDATE_BASE_URL}"

mkdir -p "$(dirname "$OUT_FILE")"

cat > "$OUT_FILE" <<EOF
key,type,encoding,value
config,namespace,,
wifi_ssid,data,string,${LEDRIVER_WIFI_SSID}
wifi_password,data,string,${LEDRIVER_WIFI_PASSWORD}
update_base_url,data,string,${LEDRIVER_UPDATE_BASE_URL}
EOF

echo "Generated $OUT_FILE"
