(function($, Drupal){
  var adjustSize = function ($element, ratio) {
    $element.height(Math.floor($element.width()/ratio));
  };
  Drupal.behaviors.drupdown_ratio = {
    attach: function(context, settings) {
      return;
      $('[data-aspect-ratio]', context).once().each(function(){
        var $element = $(this);
        var ratio = parseFloat($element.attr('data-aspect-ratio'));
        adjustSize($element, ratio);
        var timeout = false;
        $(window).resize(function(){
          if (timeout) { window.clearTimeout(timeout); }
          timeout = window.setTimeout(function(){
            adjustSize($element, ratio);
          }, 200);
        });
      });
    }
  };
}(jQuery, Drupal));
