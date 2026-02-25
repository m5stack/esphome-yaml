import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output, i2c
from esphome.const import CONF_ID, CONF_CHANNEL

CODEOWNERS = ["@Jasionf"]
DEPENDENCIES = ["pyramidrgb", "i2c"]

# 使用与小写组件一致的 C++ 命名空间与类型
pyramidrgb_ns = cg.esphome_ns.namespace("pyramidrgb")
PyramidRGBComponent = pyramidrgb_ns.class_("PyramidRGBComponent", cg.Component, i2c.I2CDevice)
PyramidRGBOutput = pyramidrgb_ns.class_("PyramidRGBOutput", output.FloatOutput)

# 父组件引用字段
CONF_PYRAMIDRGB_ID = "pyramidrgb_id"
BASE_SCHEMA = cv.Schema({
	cv.GenerateID(CONF_PYRAMIDRGB_ID): cv.use_id(PyramidRGBComponent),
})

# 颜色枚举映射
RGBColorChannel = pyramidrgb_ns.enum("RGBColorChannel", is_class=True)
COLOR_MAP = {
	"red": RGBColorChannel.COLOR_R,
	"green": RGBColorChannel.COLOR_G,
	"blue": RGBColorChannel.COLOR_B,
}

CONF_COLOR = "color"

# 每个 output 条目控制一个通道的一个颜色分量（更贴合 ESPHome output 设计）
CONFIG_SCHEMA = output.FLOAT_OUTPUT_SCHEMA.extend(
	{
		cv.Required(CONF_ID): cv.declare_id(PyramidRGBOutput),
		cv.Required(CONF_CHANNEL): cv.int_range(min=0, max=3),
		cv.Required(CONF_COLOR): cv.enum(COLOR_MAP),
	}
).extend(BASE_SCHEMA)


async def to_code(config):
	var = cg.new_Pvariable(config[CONF_ID])
	await cg.register_parented(var, config[CONF_PYRAMIDRGB_ID])
	cg.add(var.set_channel(config[CONF_CHANNEL]))
	cg.add(var.set_color(config[CONF_COLOR]))
	await output.register_output(var, config)
