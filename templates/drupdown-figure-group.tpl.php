<?php
/**
 * @file
 * Template file for Drupdown figure groups.
 *
 * $figures: Array of figures to be displayed.
 * $classes: Classes to be added to the grouping element.
 */
?>
<div class="<?php print $classes;?> clearfix">
  <?php foreach ($figures as $figure): ?>
    <?php print $figure; ?>
  <?php endforeach; ?>
</div>
