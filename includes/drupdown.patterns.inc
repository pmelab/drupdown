<?php
/**
 * @file
 * Url pattern callbacks for drupdown.
 */

/**
 * Process an url encountered.
 */
function drupdown_process_reference($url) {
  $url = preg_replace('/^\//', '', $url);
  $info = drupdown_match_pattern($url);

  if (!$info) {
    return FALSE;
  }
  return $info;
}

/**
 * Find a pattern matching the url.
 *
 * @param string $url
 *   The url to match.
 *
 * @return array
 *   Embed info.
 */
function drupdown_match_pattern(&$base_info) {
  $patterns = drupdown_get_url_patterns();
  foreach ($patterns as $id => $info) {
    if (preg_match($info['pattern'], $base_info['url'], $matches)) {
      $info['matches'] = $matches;
      $info['id'] = $id;
      $info['figure'] = FALSE;
      if (array_key_exists('file', $info)) {
        include_once $info['file'];
      }
      $base_info = array_merge($base_info, $info);
      if ($base_info['process']($base_info)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Retrieve all defined url patterns.
 */
function drupdown_get_url_patterns() {
  if ($cache = cache_get('drupdown_url_patterns')) {
    return $cache->data;
  }

  $patterns = array();
  foreach (module_implements('drupdown_url_patterns') as $module) {
    foreach (module_invoke($module, 'drupdown_url_patterns') as $key => $pattern) {
      if (!array_key_exists('process', $pattern)) {
        $pattern['process'] = $module . '_' . $key . '_reference_process';
      }
      if (!array_key_exists('pattern', $pattern)) {
        $pattern['pattern'] = '/.*/';
      }
      $patterns[$key] = $pattern;
    }
  }

  drupal_alter('drupdown_url_patterns', $patterns);
  uasort($patterns, 'drupal_sort_weight');
  cache_set('drupdown_url_patterns', $patterns);
  return $patterns;
}

/**
 * Implements hook_drupdown_url_patterns().
 */
function drupdown_drupdown_url_patterns() {
  $path = drupal_get_path('module', 'drupdown') . '/includes/patterns';
  $patterns = array();
  $patterns['link'] = array(
    'weight' => 100,
    'types' => array('link'),
    'file' => $path . '/drupdown.link.inc',
  );


  if (module_exists('oembed')) {
    $patterns['oembed'] = array(
      'weight' => 95,
      'types' => array('oembed'),
      'file' => $path . '/drupdown.oembed.inc',
    );
  }

  $patterns['download'] = array(
    'pattern' => '/^download\:\/\/(.*)/',
    'weight' => 94,
    'types' => array('link', 'download'),
    'file' => $path . '/drupdown.download.inc',
  );

  $patterns['placekitten'] = array(
    'pattern' => '/^(http):\/\/placekitten.com.*/i',
    'weight' => 90,
    'types' => array('image'),
    'file' => $path . '/drupdown.image.inc',
  );

  $patterns['image'] = array(
    'pattern' => '/^(.*?)(\:\/\/?).*\.(png|jpg|jpeg|gif)$/i',
    'weight' => 90,
    'types' => array('image'),
    'file' => $path . '/drupdown.image.inc',
  );

  $patterns['flickr'] = array(
    'pattern' => '/flickr\.com\/photos\/(.*?)\/sets\/(.*?)\//',
    'weight' => 90,
    'types' => array('gallery', 'flickr'),
    'file' => $path . '/drupdown.flickr.inc',
  );

  $patterns['google_gallery'] = array(
    'pattern' => '/plus\.google\.com\/photos\/(.+?)\/albums\/([^?&]+)/',
    'weight' => 90,
    'types' => array('gallery', 'picasa'),
    'file' => $path . '/drupdown.google_gallery.inc',
  );

  return $patterns;
}
