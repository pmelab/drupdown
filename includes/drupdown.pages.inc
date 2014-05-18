<?php
/**
 * @file
 * Drupdown test and administration pages.
 */

define('PLAYGROUND_CONTENT', <<< EOT
This is a *simple* **Testpage**.
EOT
);


/**
 * Page callback for a simple playground form for trying out drupdown.
 */
function drupdown_playground($form, &$form_state) {
  $content = _filter_drupdown_pre(@$form_state['values']['input'], FALSE);
  $content = _filter_drupdown_markdown($content, FALSE);
  $content = _filter_drupdown_post($content, FALSE);
  $form = array(
    'input' => array(
      '#type' => 'textarea',
      '#ace' => TRUE,
      '#attributes' => array(
        'data-mime' => 'text/x-drupdown',
      ),
      '#rows' => count(explode(PHP_EOL, PLAYGROUND_CONTENT)) + 2,
      '#default_value' => PLAYGROUND_CONTENT,
    ),
    'transform' => array(
      '#type' => 'button',
      '#value' => t('Transform'),
      '#ajax' => array(
        'wrapper' => 'drupdown-playground-output',
        'callback' => 'drupdown_playground_callback',
        'effect' => 'fade',
      ),
    ),
    'output' => array(
      '#prefix' => '<div id="drupdown-playground-output">',
      '#suffix' => '</div>',
      'formatted' => array(
        '#prefix' => '<div class="formatted-output clearfix">',
        '#suffix' => '</div>',
        '#markup' => $content,
      ),
      'raw' => array(
        '#prefix' => '<pre class="raw-output"><code data-mime="text/html" class="text__html">',
        '#suffix' => '</code></pre>',
        '#markup' => htmlentities($content),
      ),
    ),
  );
  return $form;
}

/**
 * Ajax callback for the playground form.
 */
function drupdown_playground_callback($form, $form_state) {
  return $form['output'];
}

/**
 * Drupdown settings form.
 */
function drupdown_settings_form($form, &$form_state) {
  $form['drupdown_flickr_api_key'] = array(
    '#title' => t('Flickr API Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('drupdown_flickr_api_key', ''),
  );

  $form['drupdown_flickr_api_private_key'] = array(
    '#title' => t('Flickr API Private Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('drupdown_flickr_api_private_key', ''),
  );

  $form['drupdown_soundcloud_client_id'] = array(
    '#title' => t('Soundcloud Client ID'),
    '#type' => 'textfield',
    '#default_value' => variable_get('drupdown_soundcloud_client_id', ''),
  );

  $form['drupdown_image_styles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Available image styles'),
    '#options' => array(
      'original' => 'original',
    ),
    '#default_value' => variable_get('drupdown_image_styles', array()),
  );

  if (module_exists('codemirror')) {
    $options = array();
    foreach (codemirror_languages() as $mime => $lang) {
      if ($mime != 'text/x-drupdown') {
        $options[$mime] = $lang['name'] . ' (' . $mime . ')';
      }
    }
    $form['drupdown_codemirror_embeds_fieldset'] = array(
      '#type' => 'fieldset',
      '#title' => t('Embedded CodeMirror modes'),
      '#description' => t('Select language modes to be attached.'),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      'drupdown_codemirror_embeds' => array(
        '#type' => 'checkboxes',
        '#options' => $options,
        '#default_value' => variable_get('drupdown_codemirror_embeds'),
      ),
    );
  }

  foreach (array_keys(image_styles()) as $style) {
    $form['drupdown_image_styles']['#options'][$style] = $style;
  }
  return system_settings_form($form);
}
