/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2019 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "fabricGrainMeter.hpp"
#include "DistrhoPluginInfo.h"
#include "fabricUI.hpp"
#include <iostream>

START_NAMESPACE_DISTRHO

fabricGrainMeter::fabricGrainMeter(Widget *widget, Size<uint> size) noexcept
    : NanoSubWidget(widget)
     ,m_meterSize(size)
{
    Window &pw = getWindow();
    pw.addIdleCallback(this, 16);

    setSize(size); 
    m_grainPlayer = &static_cast<fabricUI *>(widget)->_plugin->grainPlayer;

    grainsActiveHistory.resize(200);
}

void fabricGrainMeter::idleCallback()
{
    repaint();
}

void fabricGrainMeter::onNanoDisplay()
{
    int width = m_meterSize.getWidth();
    int height = m_meterSize.getHeight();

    //draw black widget background
    static const Color k_black(0, 0, 0);
    beginPath();
    rect(0.0f, 0.0f, getWidth(), getHeight());
    fillColor(k_black);
    fill();

    //get the current grains used count
    grainsActiveHistory.push_back(m_grainPlayer->grains_used.size());
    grainsActiveHistory.pop_front();

    //draw the grain meter
    strokeWidth(1.f);
    beginPath();
    strokeColor(Color(173, 216, 230, 255)); //FIXME (alex) make this an enum
    for (int pos = 0; pos < width; ++pos){
        moveTo(pos, height);
        lineTo(pos, height - ((float)grainsActiveHistory.at(pos) / MAX_GRAINS) * height);
    }
    stroke();
}

END_NAMESPACE_DISTRHO
