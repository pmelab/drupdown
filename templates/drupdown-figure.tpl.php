<?php
/**
 * @file
 * Template for Drupdown embedded figures.
 *
 * $content: The rendered figure content.
 * $caption: The caption to be displayed.
 * $types: Array of types the embed matches.
 */
?>
<div class="<?php print $classes; ?>"<?php print ($domid ? (' id="' . $domid . '"') : ''); ?>>
  <div class="drupdown-figure-inner">
    <div class="drupdown-figure-content"><?php print $content; ?></div>
    <?php if (!empty($caption)): ?>
      <div class="drupdown-figure-caption"><?php print $caption; ?></div>
    <?php endif; ?>
  </div>
</div>
