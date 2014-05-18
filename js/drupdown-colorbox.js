(function ($, Drupal){
  Drupal.behaviors.drupdown_colorbox = {
    attach: function (context, settings) {
      $('.drupdown-figure-group, .drupdown-gallery', context).each(function(index){
        $('a.drupdown-zoom', this).once('colorbox').colorbox({
          maxWidth: '90%',
          maxHeight: '90%',
          rel: 'gallery-' + index
        });
      });

      $('a.drupdown-image', context).each(function(){
        $(this).once('colorbox').click(function(){
          $.colorbox({
            maxWidth: '90%',
            maxHeight: '90%',
            href: $(this).attr('href')
          });
          return false;
        });
      });

      var max_width = 600;
      var max_height = 400;
      $('a.drupdown-oembed', context).each(function(){
        $(this).once('colorbox').click(function(){
          var href = $(this).attr('href');
          $.colorbox({
            innerWidth: max_width,
            innerHeight: max_height,
            href: Drupal.settings.basePath + 'drupdown/ajax-embed?url=' + href,
            onComplete: function() {
              var width = max_width;
              var height = max_height;
              $('#cboxLoadedContent iframe').each(function(){
                $(this).width(width);
                height = width / parseFloat($(this).attr('data-aspect-ratio'));
                $(this).height(height);
              });
              $(this).colorbox.resize({
                innerWidth: width,
                innerHeight: height
              });
            }
          });
          return false;
        });
      });
    }
  };
}(jQuery, Drupal));