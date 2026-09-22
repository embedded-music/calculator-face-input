#pragma once

#include <stdint.h>

struct DrumSound {
  uint8_t midiNote;
  const char* name;
};

constexpr DrumSound DRUM_SOUNDS[] = {
    {38, "Acoustic Snare"}, {40, "Electric Snare"}, {39, "Hand Clap"},
    {75, "Claves"},         {42, "Closed Hi-Hat"},  {44, "Pedal Hi-Hat"},
    {46, "Open Hi-Hat"},    {54, "Tambourine"},      {58, "Vibraslap"},
    {79, "Open Cuica"},     {55, "Splash Cymbal"},  {52, "Chinese Cymbal"},
    {51, "Ride Cymbal 1"},  {59, "Ride Cymbal 2"},  {53, "Ride Bell"},
    {56, "Cowbell"},        {67, "High Agogo"},      {68, "Low Agogo"},
    {45, "Low Tom"},        {70, "Maracas"},
};

constexpr uint8_t DRUM_SOUND_COUNT =
    sizeof(DRUM_SOUNDS) / sizeof(DRUM_SOUNDS[0]);

static_assert(DRUM_SOUND_COUNT == 20,
              "The sound layer maps one sound to each Calculator key");
