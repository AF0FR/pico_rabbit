# pico_rabbit

pico_rabbit is a Raspberry Pi Pico-based amateur radio foxhunt/rabbit controller.

It keys a radio on a timed interval, sends a short tone, sends a Morse ID/message, sends another tone, then waits until the next cycle.

Default message:

    AF0FR FOX

## Purpose

This project is intended for amateur radio fox hunts, hidden transmitter hunts, training exercises, and club demonstrations.

The design is based on the same general idea as Keyer In The Rye, but instead of acting as an interactive iambic keyer, this firmware acts as an automatic timed rabbit/fox beacon.

## Default behavior

On boot:

1. Initialize GPIO and PWM audio.
2. Wait for the arming delay.
3. Key PTT.
4. Send a pre-ID tone.
5. Send the configured Morse message.
6. Send a post-ID tone.
7. Release PTT.
8. Wait until the next interval.
9. Repeat.

Default cycle:

    2 sec tone
    AF0FR FOX in Morse
    3 sec tone
    Idle until 60 seconds total cycle time
    Repeat

## Default pins

| Function | Pico GPIO |
|---|---:|
| PTT | GPIO15 |
| PWM audio tone | GPIO16 |
| CW key output | GPIO17 |
| LED | Pico onboard LED |

## Build requirements

You need the Raspberry Pi Pico SDK and the ARM GCC toolchain.

On Fedora:

    sudo dnf install -y cmake make gcc gcc-c++ git python3 arm-none-eabi-gcc-cs arm-none-eabi-newlib

Install the Pico SDK:

    mkdir -p ~/pico
    cd ~/pico
    git clone https://github.com/raspberrypi/pico-sdk.git
    cd pico-sdk
    git submodule update --init --recursive

Set the SDK path:

    export PICO_SDK_PATH=$HOME/pico/pico-sdk

To make it permanent:

    echo 'export PICO_SDK_PATH=$HOME/pico/pico-sdk' >> ~/.bashrc
    source ~/.bashrc

## Build

    cd ~/code/pico_rabbit
    rm -rf build
    mkdir build
    cd build
    cmake ..
    make -j$(nproc)

The UF2 output should be:

    build/pico_rabbit.uf2

## Flashing

Hold the Pico BOOTSEL button while plugging it into USB.

Then copy the UF2 file to the mounted Pico drive:

    cp build/pico_rabbit.uf2 /run/media/$USER/RPI-RP2/

The Pico will reboot and start running the firmware.

## Configuration

Main settings are in:

    include/config.h

Common values to change:

    #define CALLSIGN              "AF0FR"
    #define RABBIT_MESSAGE        "AF0FR FOX"
    #define WPM                   15
    #define TONE_HZ               700
    #define RABBIT_INTERVAL_MS    60000
    #define PRE_ID_TONE_MS        2000
    #define POST_ID_TONE_MS       3000

## Suggested radio interface

For PTT using a 2N3904 transistor:

    Pico GPIO15 -> 4.7k resistor -> 2N3904 base
    2N3904 emitter -> radio ground
    2N3904 collector -> radio PTT line
    Pico GND -> radio ground

For audio into an HT mic input, start with attenuation:

    GPIO16 PWM audio -> 0.1 uF capacitor -> 10k series resistor -> radio mic input
    Pico GND -----------------------------------------------> radio ground

Do not feed the Pico PWM pin directly into the radio mic input at full level. Start with low audio and increase only if needed.

## Operating notes

Use low power for a beginner fox hunt. Too much signal makes the hunt harder because nearby receivers can overload or hear the signal from every direction.

Recommended starting setup:

    Radio power: low
    Antenna: rubber duck or intentionally limited radiator
    Cycle: 10-ish seconds transmitting / 50-ish seconds silent
    Message: callsign included every cycle

## Legal note

When used on amateur radio frequencies, the transmitter is still an amateur station. A licensed control operator is responsible for the station, and the callsign should be transmitted as part of the cycle.

This project is configured to send the callsign in every transmit cycle.
