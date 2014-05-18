<?php
/**
 * @file
 * Generate a download link for the given file.
 */

/**
 * Implements hook_REFERENCE_TYPE_callback().
 */
function drupdown_download_reference_process(&$info) {
  $info['url'] = file_create_url(str_replace('download://', 'public://', $info['url']));
  $info['content'] = l($info['alt'], $info['url'], array(
    'attributes' => array(
      'download' => 'true',
      'title' => $info['title'] ? $info['title'] : $info['alt'],
      'class' => array('drupdown-download'),
    ),
  ));
  return TRUE;
}
