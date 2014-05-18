<?php
/**
 * Template to display external links as preview embeds.
 */
?>
<div class="drupdown-link-embed">
  <?php if ($description && $image): ?>
    <h2>
      <?php print l($title, $url, array(
        'external' => TRUE,
        'attributes' => array(
          'title' => $title,
        ))); ?>
    </h2>

    <?php print l($domain, $url, array(
      'external' => TRUE,
      'attributes' => array(
        'title' => $title,
        'class' => array('drupdown-link-domain'),
      ))); ?>
    <?php print l(theme('image', array(
        'path' => $image,
        'alt' => $title,
      )), $url, array(
        'html' => TRUE,
        'external' => TRUE,
        'attributes' => array(
          'title' => $title,
          'class' => array('drupdown-link-image'),
        ))); ?>
    <p class="drupdown-link-description"><?php print $description; ?></p>
  <?php else: ?>
    <?php print l($title, $url, array(
      'external' => TRUE,
      'attributes' => array(
        'title' => $title,
        'class' => array('drupdown-link-solo'),
      ))); ?>
  <?php endif; ?>
</div>
