<?php
/**
 * @file
 * Drupdown user interface enhancements.
 */

/**
 * Implements hook_ace_plugins_alter().
 */
function drupdown_ace_plugins_alter(&$plugins) {
  $plugins['modes']['drupdown/mode'] = array(
    'title' => 'Drupdown',
  );
  $plugins['toolbars']['drupdown/toolbar'] = array(
    'title' => 'Drupdown',
    'attached' => array(
      'js' => array(
        drupal_get_path('module', 'drupdown') . '/js/drupdown-toolbar.js',
      ),
      'css' => array(
        drupal_get_path('module', 'drupdown') . '/css/drupdown-toolbar.css',
      ),
    ),
  );
}

/**
 * Implements hook_element_info_alter().
 */
function drupdown_element_info_alter(&$types) {
  $types['managed_file']['#process'][] = '_drupdown_file_element_process';
}

/**
 * Element process callback for altering the file element.
 */
function _drupdown_file_element_process($element) {
  $zeropath = libraries_get_path('ZeroClipboard');
  if (file_exists($zeropath . '/ZeroClipboard.js')) {
    $element['#attached']['css'][] = drupal_get_path('module', 'drupdown') . '/css/drupdown-clipboard.css';
    $element['#attached']['js'][] = $zeropath . '/ZeroClipboard.js';
    $element['#attached']['js'][] = drupal_get_path('module', 'drupdown') . '/js/drupdown-clipboard.js';
  }
  if (array_key_exists('#file', $element) && is_object($element['#file'])) {
    $element['drupdown_copy_links'] = array(
      '#weight' => 20,
      '#theme' => 'item_list',
      '#items' => array(),
      '#attributes' => array(
        'class' => array('drupdown-copy-links'),
      ),
    );
    $element['drupdown_copy_links']['#items']['original'] = l(t('original'), file_create_url($element['#file']->uri), array(
      'attributes' => array(
        'data-clipboard-text' => $element['#file']->uri,
      ),
    )) . ': ' . $element['#file']->uri;

    if (in_array('image_field_widget_process', $element['#process'])) {
      $styles = variable_get('drupdown_image_styles', array());
      foreach (array_keys(image_styles()) as $name) {
        if (array_key_exists($name, $styles) && $styles[$name]) {
          $element['drupdown_copy_links']['#items'][$name] = l($name, image_style_url($name, $element['#file']->uri), array(
            'attributes' => array(
              'data-clipboard-text' => str_replace('public', $name, $element['#file']->uri),
            ),
          )) . ': ' . str_replace('public', $name, $element['#file']->uri);
        }
      }
    }

    $download_uri = str_replace('public://', 'download://', $element['#file']->uri);
    $element['drupdown_copy_links']['#items']['download'] = l(t('download'), file_create_url($element['#file']->uri), array(
        'attributes' => array(
          'data-clipboard-text' => $download_uri,
        ),
      )) . ': ' . $download_uri;
  }
  return $element;
}
