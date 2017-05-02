<?php

/**
 * @file
 * Documents hooks provided by the ArCa module.
 */

/**
 * Allows modules to alter the data array used to create a ArCa IPay query.
 *
 * @param &$data
 *   The data array used to create redirect form elements.
 * @param $order
 *   The full order object the redirect form is being generated for.
 *
 */
function hook_commerce_arca_ipay_order_form_data_alter(&$data, $order) {
  // Change data or add some parameters which will be sent to arca server.
}
