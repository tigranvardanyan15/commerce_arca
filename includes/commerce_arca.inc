<?php

/**
 * @file
 * Helper functions for Commerce ArCa module.
 */

/**
 * Add url to form with deviding get query to hidden elements.
 */
function _commerce_arca_ipay_add_redirect_url(&$form, $url) {
  $url = explode('?', $url);
  $form['#action'] = $url[0];

  if (isset($url[1])) {
    $query = explode('&', $url[1]);

    foreach ($query as $q) {
      $v = explode('=', $q);

      $form[$v[0]] = array(
        '#type' => 'hidden',
        '#value' => $v[1],
      );
    }
  }
}

/**
 * Makes http request to arca server and returns result as json in case of success and NULL otherwise.
 */
function _commerce_arca_ipay_make_request($query_function, $data) {
  $payment_method = commerce_payment_method_instance_load('arca_ipay|commerce_payment_arca_ipay');
  $settings = $payment_method['settings'];

  $data = $data + array(
    'userName' => $settings['api_login'],
    'password' => $settings['api_password'],
  );

  $context_options = array();
  if ($settings['test_mode'] == 1) {
    // If test mode is enabled not verify ssl.
    $context_options['ssl'] = array(
      'verify_peer' => FALSE,
      'verify_peer_name' => FALSE,
    );
  }

  // If there is no ending slash add it.
  if (strrpos($settings['api_url'], '/') != strlen($settings['api_url']) - 1) {
    $settings['api_url'] .= '/';
  }

  // Send http request to arca server for registering order and retrieving back url.
  $request_url = $settings['api_url'] . $query_function . '.do?' . http_build_query($data);
  $result = drupal_http_request($request_url, array(
    'context' => stream_context_create($context_options),
  ));

  if ($result->code == 200) {
    $result = json_decode($result->data);
  }
  else {
    $result = NULL;
  }

  return $result;
}

/**
 * Returns the returnUrl for arca.
 *
 * @param $method_id
 *   Optionally specify a payment method instance ID to include in the URL.
 */
function _commerce_arca_return_url($instance_id = NULL) {
  $parts = array(
    'commerce_arca',
  );

  if (!empty($instance_id)) {
    $parts[] = $instance_id;
  }

  return url(implode('/', $parts), array('absolute' => TRUE));
}
