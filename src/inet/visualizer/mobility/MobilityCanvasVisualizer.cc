//
// Copyright (C) 2020 OpenSim Ltd.
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

#include "inet/visualizer/mobility/MobilityCanvasVisualizer.h"

#include "inet/common/ModuleAccess.h"

namespace inet {

namespace visualizer {

Define_Module(MobilityCanvasVisualizer);

MobilityCanvasVisualizer::MobilityCanvasVisualization::~MobilityCanvasVisualization()
{
    delete positionFigure;
    delete orientationFigure;
    delete velocityFigure;
    delete trailFigure;
}

MobilityCanvasVisualizer::MobilityCanvasVisualization::MobilityCanvasVisualization(cOvalFigure *positionFigure, cPieSliceFigure *orientationFigure, cLineFigure *velocityFigure, TrailFigure *trailFigure, IMobility *mobility) :
    MobilityVisualization(mobility),
    positionFigure(positionFigure),
    orientationFigure(orientationFigure),
    velocityFigure(velocityFigure),
    trailFigure(trailFigure)
{
}

void MobilityCanvasVisualizer::initialize(int stage)
{
    MobilityVisualizerBase::initialize(stage);
    if (!hasGUI()) return;
    if (stage == INITSTAGE_LOCAL) {
        zIndex = par("zIndex");
        canvasProjection = CanvasProjection::getCanvasProjection(visualizationTargetModule->getCanvas());
    }
}

void MobilityCanvasVisualizer::refreshDisplay() const
{
    MobilityVisualizerBase::refreshDisplay();
    for (auto it : mobilityVisualizations) {
        auto mobilityVisualization = static_cast<MobilityCanvasVisualization *>(it.second);
        auto mobility = mobilityVisualization->mobility;
        auto position = canvasProjection->computeCanvasPoint(mobility->getCurrentPosition());
        auto orientation = mobility->getCurrentAngularPosition();
        auto velocity = canvasProjection->computeCanvasPoint(mobility->getCurrentVelocity());
        if (displayPositions) {
            double radius = positionCircleRadius / getEnvir()->getZoomLevel(visualizationTargetModule);
            mobilityVisualization->positionFigure->setBounds(cFigure::Rectangle(position.x - radius, position.y - radius, 2 * radius, 2 * radius));
        }
        if (displayOrientations) {
            // NOTE: this negation cancels out the (incorrect) CCW angle handling of cPieSliceFigure (see bug https://dev.omnetpp.org/bugs/view.php?id=1030)
            auto angle = -orientation.toEulerAngles().alpha;
            mobilityVisualization->orientationFigure->setStartAngle(rad(angle - rad(M_PI) * orientationPieSize).get());
            mobilityVisualization->orientationFigure->setEndAngle(rad(angle + rad(M_PI) * orientationPieSize).get());
            double radius = orientationPieRadius / getEnvir()->getZoomLevel(visualizationTargetModule);
            mobilityVisualization->orientationFigure->setBounds(cFigure::Rectangle(position.x - radius, position.y - radius, 2 * radius, 2 * radius));
        }
        if (displayVelocities) {
            mobilityVisualization->velocityFigure->setStart(position);
            mobilityVisualization->velocityFigure->setEnd(position + velocity * velocityArrowScale);
            mobilityVisualization->velocityFigure->setVisible(velocity.getLength() != 0);
        }
        if (displayMovementTrails)
            extendMovementTrail(mobility, mobilityVisualization->trailFigure, position);
    }
    visualizationTargetModule->getCanvas()->setAnimationSpeed(mobilityVisualizations.empty() ? 0 : animationSpeed, this);
}

MobilityCanvasVisualizer::MobilityCanvasVisualization *MobilityCanvasVisualizer::getMobilityVisualization(const IMobility *mobility) const
{
    auto it = mobilityVisualizations.find(mobility);
    if (it == mobilityVisualizations.end())
        return nullptr;
    else
        return static_cast<MobilityCanvasVisualization *>(it->second);
}

void MobilityCanvasVisualizer::addMobilityVisualization(const IMobility *mobility, MobilityCanvasVisualization *mobilityVisualization)
{
    mobilityVisualizations[mobility] = mobilityVisualization;
    auto canvas = visualizationTargetModule->getCanvas();
    if (displayPositions)
        canvas->addFigure(mobilityVisualization->positionFigure);
    if (displayOrientations)
        canvas->addFigure(mobilityVisualization->orientationFigure);
    if (displayVelocities)
        canvas->addFigure(mobilityVisualization->velocityFigure);
    if (displayMovementTrails)
        canvas->addFigure(mobilityVisualization->trailFigure);
}

void MobilityCanvasVisualizer::removeMobilityVisualization(const MobilityVisualization *mobilityVisualization)
{
    auto canvas = visualizationTargetModule->getCanvas();
    auto mobilityCanvasVisualization = static_cast<const MobilityCanvasVisualization *>(mobilityVisualization);
    if (displayPositions)
        canvas->removeFigure(mobilityCanvasVisualization->positionFigure);
    if (displayOrientations)
        canvas->removeFigure(mobilityCanvasVisualization->orientationFigure);
    if (displayVelocities)
        canvas->removeFigure(mobilityCanvasVisualization->velocityFigure);
    if (displayMovementTrails)
        canvas->removeFigure(mobilityCanvasVisualization->trailFigure);
    MobilityVisualizerBase::removeMobilityVisualization(mobilityVisualization);
}

MobilityCanvasVisualizer::MobilityCanvasVisualization *MobilityCanvasVisualizer::createMobilityVisualization(IMobility *mobility)
{
    auto module = const_cast<cModule *>(check_and_cast<const cModule *>(mobility));
    cOvalFigure *positionFigure = nullptr;
    if (displayPositions) {
        positionFigure = new cOvalFigure("position");
        positionFigure->setTags((std::string("position ") + tags).c_str());
        positionFigure->setTooltip("This circle represents the current position of the mobility model");
        positionFigure->setZIndex(zIndex);
        positionFigure->setLineColor(positionCircleLineColorSet.getColor(module->getId()));
        positionFigure->setLineWidth(positionCircleLineWidth);
        positionFigure->setFilled(true);
        positionFigure->setFillColor(positionCircleFillColorSet.getColor(module->getId()));
    }
    cPieSliceFigure *orientationFigure = nullptr;
    if (displayOrientations) {
        orientationFigure = new cPieSliceFigure("orientation");
        orientationFigure->setTags((std::string("orientation ") + tags).c_str());
        orientationFigure->setTooltip("This arc represents the current orientation of the mobility model");
        orientationFigure->setZIndex(zIndex);
        orientationFigure->setLineOpacity(orientationPieOpacity);
        orientationFigure->setLineColor(orientationLineColor);
        orientationFigure->setLineStyle(orientationLineStyle);
        orientationFigure->setLineWidth(orientationLineWidth);
        orientationFigure->setFilled(true);
        orientationFigure->setFillOpacity(orientationPieOpacity);
        orientationFigure->setFillColor(orientationFillColor);
    }
    cLineFigure *velocityFigure = nullptr;
    if (displayVelocities) {
        velocityFigure = new cLineFigure("velocity");
        velocityFigure->setTags((std::string("velocity ") + tags).c_str());
        velocityFigure->setTooltip("This arrow represents the current velocity of the mobility model");
        velocityFigure->setZIndex(zIndex);
        velocityFigure->setVisible(false);
        velocityFigure->setEndArrowhead(cFigure::ARROW_SIMPLE);
        velocityFigure->setLineColor(velocityLineColor);
        velocityFigure->setLineStyle(velocityLineStyle);
        velocityFigure->setLineWidth(velocityLineWidth);
    }
    TrailFigure *trailFigure = nullptr;
    if (displayMovementTrails) {
        trailFigure = new TrailFigure(trailLength, true, "movement trail");
        trailFigure->setTags((std::string("movement_trail recent_history ") + tags).c_str());
        trailFigure->setZIndex(zIndex);
    }
    return new MobilityCanvasVisualization(positionFigure, orientationFigure, velocityFigure, trailFigure, mobility);
}

void MobilityCanvasVisualizer::extendMovementTrail(const IMobility *mobility, TrailFigure *trailFigure, cFigure::Point position) const
{
    cFigure::Point startPosition;
    cFigure::Point endPosition = position;
    if (trailFigure->getNumFigures() == 0)
        startPosition = position;
    else
        startPosition = static_cast<cLineFigure *>(trailFigure->getFigure(trailFigure->getNumFigures() - 1))->getEnd();
    double dx = startPosition.x - endPosition.x;
    double dy = startPosition.y - endPosition.y;
    // TODO 1?
    if (trailFigure->getNumFigures() == 0 || dx * dx + dy * dy > 1) {
        cLineFigure *movementLine = new cLineFigure("movementTrail");
        movementLine->setTags((std::string("movement_trail recent_history ") + tags).c_str());
        movementLine->setTooltip("This line represents the recent movement trail of the mobility model");
        movementLine->setStart(startPosition);
        movementLine->setEnd(endPosition);
        auto module = const_cast<cModule *>(check_and_cast<const cModule *>(mobility));
        movementLine->setLineColor(movementTrailLineColorSet.getColor(module->getId()));
        movementLine->setLineStyle(movementTrailLineStyle);
        movementLine->setLineWidth(movementTrailLineWidth);
        movementLine->setZoomLineWidth(false);
        trailFigure->addFigure(movementLine);
    }
}

void MobilityCanvasVisualizer::receiveSignal(cComponent *source, simsignal_t signal, cObject *object, cObject *details)
{
    Enter_Method("receiveSignal");
    if (signal == IMobility::mobilityStateChangedSignal) {
        if (moduleFilter.matches(check_and_cast<cModule *>(source))) {
            auto mobility = dynamic_cast<IMobility *>(source);
            auto mobilityVisualization = getMobilityVisualization(mobility);
            if (mobilityVisualization == nullptr) {
                mobilityVisualization = createMobilityVisualization(mobility);
                addMobilityVisualization(mobility, mobilityVisualization);
            }
        }
    }
    else if (signal == PRE_MODEL_CHANGE) {
        if (dynamic_cast<cPreModuleDeleteNotification *>(object)) {
            if (auto mobility = dynamic_cast<IMobility *>(source)) {
                auto mobilityVisualization = getMobilityVisualization(mobility);
                removeMobilityVisualization(mobilityVisualization);
                delete mobilityVisualization;
            }
        }
    }
    else
        throw cRuntimeError("Unknown signal");
}

} // namespace visualizer

} // namespace inet

