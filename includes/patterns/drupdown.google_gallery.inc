<?php
/**
 * @file
 * Picasa embed processing.
 */

/**
 * Implements hook_REFERENCE_TYPE_callback().
 */
function drupdown_google_gallery_reference_process(&$info) {
  $matches = $info['matches'];
  $user_id = $matches[1];
  $album = $matches[2];
  $url = 'https://picasaweb.google.com/data/feed/api/user/' . $user_id . '/albumid/' . $album . '?alt=json&thumbsize=72c';
  $result = drupal_http_request($url);
  $data = drupal_json_decode($result->data);
  if (!$data) {
    return FALSE;
  }
  $list = array();
  foreach ($data['feed']['entry'] as $img) {
    $list[] = l(theme('image', array(
      'path' => $img['media$group']['media$thumbnail'][0]['url'],
      'title' => $img['title']['$t'],
    )), $img['content']['src'], array(
      'attributes' => array(
        'class' => array('drupdown-zoom'),
        'rel' => $img['gphoto$albumid']['$t'],
        'title' => $img['title']['$t'],
      ),
      'html' => TRUE,
    ));
  }
  if (count($list) == 0) {
    return FALSE;
  }
  $info['content'] = theme('item_list', array(
    'items' => $list,
    'attributes' => array(
      'class' => 'drupdown-picasa-album',
    ),
  ));
  return TRUE;
}
