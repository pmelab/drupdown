<?php
/**
 * @file
 * Drupdown oEmbed integration.
 */

/**
 * Process oEmbed includes.
 */
function drupdown_oembed_reference_process(&$info) {
  $data = oembed_get_data($info['url']);
  if (!$data) {
    return FALSE;
  }
  $info['title'] = $data['title'];
  $info['content'] = $data['html'];
  $info['types'][] = $data['type'];
  $info['types'][] = $data['provider'];
  $info['figure'] = TRUE;
  return TRUE;
}
