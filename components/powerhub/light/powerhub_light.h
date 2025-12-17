#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/components/light/light_effect.h"
#include "esphome/components/light/light_state.h"
#include "../powerhub.h"

namespace esphome {
namespace powerhub {

class USBCLight : public light::LightOutput,
                  public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

class USBALight : public light::LightOutput,
                  public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

class GroveBlueLight : public light::LightOutput,
                       public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

class GroveRedLight : public light::LightOutput,
                      public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

class RS485CANLight : public light::LightOutput,
                      public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

class BATChargeLight : public light::LightOutput,
                       public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

class PowerLeftLight : public light::LightOutput,
                       public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

class PowerRightLight : public light::LightOutput,
                        public Parented<PowerHub>
{
public:
    light::LightTraits get_traits() override;
    void write_state(light::LightState *state) override;
};

    
} // namespace powerhub
} // namespace esphome
