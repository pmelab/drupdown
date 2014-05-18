<?php
/**
 * @file
 * Process simple links.
 */

/**
 * Implements hook_REFERENCE_TYPE_callback().
 */
function drupdown_link_reference_process(&$info) {
  $info['content'] = l($info['alt'], $info['url']);
  if (preg_match('/^mailto\:|^#/', $info['url'])) {
    $info['types'][] = 'mailto';
    return TRUE;
  }

  $public_file = preg_match('/^(public|original)\:/', $info['url']);
  if ($public_file) {
    $info['url'] = file_create_url($info['url']);
    $info['types'][] = 'internal';
  }
  else if (!url_is_external($info['url'])) {
    $info['url'] = url($info['url']);
    $info['types'][] = 'internal';
  }

  $info['content'] = l($info['alt'], $info['url'], array(
    'attributes' => array(
      'title' => $info['title'] ? $info['title'] : $info['alt'],
    ),
  ));
  return TRUE;
}
