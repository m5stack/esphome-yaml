import esphome.codegen as cg
from esphome.components import espnow

DEPENDENCIES = ["espnow"]
CODEOWNERS = ["@jason"]

espnow_switch_ns = cg.esphome_ns.namespace("espnow_switch")

# 常量定义
CONF_ESPNOW_ID = "espnow_id"
CONF_MAC_ADDRESS = "mac_address"
CONF_RESPONSE_TOKEN = "response_token"
CONF_RETRY_COUNT = "retry_count"
CONF_RETRY_INTERVAL = "retry_interval"
