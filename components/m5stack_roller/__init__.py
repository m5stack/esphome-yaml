import esphome.codegen as cg
import esphome.config_validation as cv

CODEOWNERS = ["@ItsRebaseTime"]
IS_PLATFORM_COMPONENT = True

m5stack_roller_ns = cg.esphome_ns.namespace("m5stack_roller")
RollerBase = m5stack_roller_ns.class_("RollerBase")

CONF_ROLLER_BASE_ID = "m5stack_roller_id"

ROLLER_BASE_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_ROLLER_BASE_ID): cv.use_id(RollerBase),
    }
)
