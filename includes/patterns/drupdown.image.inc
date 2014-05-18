<?php
/**
 * @file
 * Image embed processing.
 */

/**
 * Pass placekitten requests to image handling.
 */
function drupdown_placekitten_reference_process(&$info) {
  return drupdown_image_reference_process($info);
}

/**
 * Implements hook_REFERENCE_TYPE_callback().
 */
function drupdown_image_reference_process(&$info) {
  $matches = $info['matches'];
  $url = $matches[0];
  $protocol = $matches[1];
  $info['figure'] = TRUE;
  if ($protocol == 'original') {
    $protocol = 'public';
    $url = str_replace('original://', 'public://', $url);
  }
  $original_url = $url;
  if (!in_array($protocol, array('http', 'https', 'public', 'private'))) {
    if (in_array($protocol, array_keys(image_styles()))) {
      $pub = str_replace($protocol . '://', 'public://', $url);
      $style_url = image_style_url($protocol, $pub);
      $info['types'][] = $protocol;
      $original_url = file_create_url($pub);
      $url = $style_url;
    }
    else {
      return FALSE;
    }
  }
  $classes = array();
  if (!$info['title']) {
    $classes[] = 'drupdown-image-no-title';
  }
  if (!$info['alt']) {
    $classes[] = 'drupdown-image-no-alt';
  }
  $img = theme('image', array(
    'path' => $url,
    'title' => $info['title'],
    'alt' => $info['alt'],
    'attributes' => array(
      'class' => $classes,
    ),
  ));


  $info['url'] = $original_url;
  if ($matches[1] != 'public') {
    $types[] = 'zoomable';
    $img = l($img, $original_url, array(
      'html' => TRUE,
      'attributes' => array(
        'class' => 'drupdown-zoom',
        'title' => $info['title'],
      ),
    ));
  }
  $info['content'] = $img;

  return TRUE;
}
