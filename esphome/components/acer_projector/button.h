#pragma once

#include "esphome/components/button/button.h"
#include "acer_projector.h"

namespace esphome {
namespace acer_projector {

class AcerProjector;

/**
 * Hide display button
 * 
 * Sends the hide command (IR code 030) to the projector,
 * which toggles the screen blank/hide feature.
 */
class AcerProjectorHideButton : public button::Button {
 public:
  AcerProjectorHideButton(AcerProjector *parent) : parent_(parent) {}
  void press_action() override;

 protected:
  AcerProjector *parent_;
};

/**
 * Freeze display button
 * 
 * Sends the freeze command (IR code 007) to the projector,
 * which pauses/freezes the current image on screen.
 */
class AcerProjectorFreezeButton : public button::Button {
 public:
  AcerProjectorFreezeButton(AcerProjector *parent) : parent_(parent) {}
  void press_action() override;

 protected:
  AcerProjector *parent_;
};

/**
 * Keystone adjustment up button
 * 
 * Sends the keystone up command (IR code 042) to the projector,
 * which adjusts the vertical keystone correction.
 */
class AcerProjectorKeystoneUpButton : public button::Button {
 public:
  AcerProjectorKeystoneUpButton(AcerProjector *parent) : parent_(parent) {}
  void press_action() override;

 protected:
  AcerProjector *parent_;
};

/**
 * Keystone adjustment down button
 * 
 * Sends the keystone down command (IR code 043) to the projector,
 * which adjusts the vertical keystone correction.
 */
class AcerProjectorKeystoneDownButton : public button::Button {
 public:
  AcerProjectorKeystoneDownButton(AcerProjector *parent) : parent_(parent) {}
  void press_action() override;

 protected:
  AcerProjector *parent_;
};

}  // namespace acer_projector
}  // namespace esphome 