#include <M5StX.h>
#include "Application.h"
#include "SpectrogramConfig.h"


Application::Application(TFT_eSPI &display, SDPSensor& sensor) : m_ui(display, SPECTROGRAM_WINDOW_SIZE, SPECTROGRAM_HISTORY_CHUNKS), m_processor(SPECTROGRAM_WINDOW_SIZE, SPECTROGRAM_HISTORY_CHUNKS), m_sdp_sampler(sensor)
{
}

bool Application::begin()
{
  return m_sdp_sampler.begin();
}

void Application::stop()
{
  m_sdp_sampler.stop();
}

void Application::process_samples()
{
  // grab the samples
  const int16_t *samples = m_sdp_sampler.getCapturedAudioBuffer();
  if (samples) {
    m_processor.update(samples);
    m_ui.update(m_processor.fft_input, m_processor.energy, m_processor.energy_size);
  }
}

void Application::loop()
{
  process_samples();
  M5.BtnB.read();
  if (M5.BtnB.wasPressed()) {
    m_ui.toggle_display();
  }
}