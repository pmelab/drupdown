<?php
/**
 * @file
 * Flickr embed processing.
 */

/**
 * Implements hook_REFERENCE_TYPE_callback().
 */
function drupdown_flickr_reference_process(&$info) {
  $matches = $info['matches'];

  $api_key = variable_get('drupdown_flickr_api_key', FALSE);
  if (!$api_key) {
    return FALSE;
  }
  $set = $matches[2];
  $url = "http://api.flickr.com/services/rest/?method=flickr.photosets.getPhotos&api_key=$api_key&photoset_id=$set&format=php_serial";
  $result = drupal_http_request($url);
  $images = array();
  if ($result->data) {
    $set = unserialize($result->data);
    foreach ($set['photoset']['photo'] as $photo) {
      $thumb = "http://farm{$photo['farm']}.staticflickr.com/{$photo['server']}/{$photo['id']}_{$photo['secret']}_s.jpg";
      $large = "http://farm{$photo['farm']}.staticflickr.com/{$photo['server']}/{$photo['id']}_{$photo['secret']}_c.jpg";
      $images[] = l(theme('image', array(
        'path' => $thumb,
        'title' => $photo['title'],
      )), $large, array(
        'attributes' => array(
          'class' => array('drupdown-zoom'),
          'rel' => $set['photoset']['id'],
          'title' => $photo['title'],
        ),
        'html' => TRUE,
      ));
    }
    if (count($images) == 0) {
      return FALSE;
    }
    $info['content'] = theme('item_list', array(
      'attributes' => array(
        'class' => 'drupdown-flickr-album',
      ),
      'items' => $images,
    ));
    return TRUE;
  }
  return FALSE;
}
