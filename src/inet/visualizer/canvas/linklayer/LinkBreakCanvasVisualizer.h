//
// Copyright (C) 2020 OpenSim Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#ifndef __INET_LINKBREAKCANVASVISUALIZER_H
#define __INET_LINKBREAKCANVASVISUALIZER_H

#include "inet/common/geometry/common/CanvasProjection.h"
#include "inet/visualizer/base/LinkBreakVisualizerBase.h"

namespace inet {

namespace visualizer {

class INET_API LinkBreakCanvasVisualizer : public LinkBreakVisualizerBase
{
  protected:
    class INET_API LinkBreakCanvasVisualization : public LinkBreakVisualization {
      public:
        cIconFigure *figure = nullptr;

      public:
        LinkBreakCanvasVisualization(cIconFigure *figure, int transmitterModuleId, int receiverModuleId);
        virtual ~LinkBreakCanvasVisualization() { delete figure; }
    };

  protected:
    double zIndex = NaN;
    const CanvasProjection *canvasProjection = nullptr;
    cGroupFigure *linkBreakGroup = nullptr;

  protected:
    virtual void initialize(int stage) override;
    virtual void refreshDisplay() const override;

    virtual const LinkBreakVisualization *createLinkBreakVisualization(cModule *transmitter, cModule *receiver) const override;
    virtual void addLinkBreakVisualization(const LinkBreakVisualization *linkBreakVisualization) override;
    virtual void removeLinkBreakVisualization(const LinkBreakVisualization *linkBreakVisualization) override;
    virtual void setAlpha(const LinkBreakVisualization *linkBreakVisualization, double alpha) const override;
};

} // namespace visualizer

} // namespace inet

#endif

