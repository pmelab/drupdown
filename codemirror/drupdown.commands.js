(function(){

  // Continue markown lists.
  CodeMirror.commands.drupdownContinueList = function(cm) {
    var pos = cm.getCursor(), token = cm.getTokenAt(pos);
    var space;
    if (token.className == "string" || token.className == "quote") {
      var full = cm.getRange({line: pos.line, ch: 0}, {line: pos.line, ch: token.end});
      var listStart = /\s*[>+|\*|\-|\+|\d+\.]/, listContinue;
      if (cm.getLine(pos.line).search(listStart) == 0) {
        var reg = /^[\W]*(\d+)\./g;
        var matches = reg.exec(full);
        if(matches)
          listContinue = (parseInt(matches[1]) + 1) + ". ";
        else
          var reg = /^[\W]*([>+|\*|\-|\+])/g;
        var matches = reg.exec(full);
        if (matches) {
          listContinue = matches[1] + " ";
        }
        space = full.slice(0, token.start);
        space = full.replace(/^(\s*).*/, '$1');
        console.log(space + ' - ' + space.length);
        if (!/^\s*$/.test(space)) {
          space = "";
          for (var i = 0; i < token.start; ++i) space += " ";
        }
      }
    }
    if (space != null && listContinue)
      cm.replaceSelection("\n" + space + listContinue, "end");
    else
      cm.execCommand("newlineAndIndent");
  };
  // Insert and increase headlines.
  CodeMirror.commands.drupdownHeadline = function (cm) {
    var pos = cm.getCursor();
    var line = cm.getLine(pos.line);
    var next = cm.getLine(pos.line + 1);

    // Do nothing on an empty line.
    if (line.match(/^\s*$/)) { return; }

    if (line.match(/^[-=]{3,}\s*$/)) {
      // We are in the second line of an settext header.
      // Make previous line the current line.
      if (pos.line === 0) {
        return;
      }
      pos.line--;
      next = line;
      line = cm.getLine(pos.line);
      if (line.match(/^\s*$/)) {
        return;
      }
    }

    var range = { from: { line: pos.line, ch: 0 }};
    if (next && next.match(/^[-=]{3,}\s*$/)) {
      range.to = { line: pos.line + 1, ch: next.length };
    }
    else {
      range.to = { line: pos.line, ch: line.length };
    }
    var value = cm.getRange(range.from, range.to);
    var matches = /^(#+\s*)?([^\s].*)(\n[-=]{3,})?$/.exec(value);

    var level = 0;

    if (matches[3] !== undefined && matches[3].match(/\n={3,}\s*/)) {
      level = 1;
    }
    else if (matches[3] !== undefined && matches[3].match(/\n-{3,}\s*/)) {
      level = 2;
    }
    else if (matches[1] !== undefined) {
      level = matches[1].replace(' ', '').length;
    }

    var headline = matches[2];
    level++;
    if (level > 5) {
      level = 0;
    }
    switch (level) {
      case 1:
        headline = headline + "\n" + (new Array(headline.length + 1)).join('=');
        break;

      case 2:
        headline = headline + "\n" + (new Array(headline.length + 1)).join('-');
        break;

      case 0:
        break;

      default:
        headline = (new Array(level + 1)).join('#') + ' ' + headline;
        break;
    }
    cm.replaceRange(headline, range.from, range.to);
  };

  // Add/Increase emphasizes.
  CodeMirror.commands.drupdownEmphasize = function (cm, strong) {
    var range = {};

    if (cm.somethingSelected()) {
      range.from = cm.getCursor('start');
      range.to = cm.getCursor('end');
    }
    else {
      var pos = cm.getCursor();
      var token = cm.getTokenAt(pos);
      range.from = { line: pos.line, ch: token.start };
      range.to = { line: pos.line, ch: token.end };
    }

    var text = cm.getRange(range.from, range.to);
    if (strong) {
      text = '**' + text + '**';
    }
    else {
      text = '*' + text + '*';
    }
    cm.replaceRange(text, range.from, range.to);
  };

  CodeMirror.commands.drupdownStrong = function (cm) {
    CodeMirror.commands.drupdownEmphasize(cm, true)
  }

  CodeMirror.commands.drupdownUnorderedList = function (cm, ordered) {
    var range = {};

    if (cm.somethingSelected()) {
      range.from = cm.getCursor('start');
      range.to = cm.getCursor('end');
    }
    else {
      var pos = cm.getCursor();
      var token = cm.getTokenAt(pos);
      range.from = { line: pos.line, ch: token.start };
      range.to = { line: pos.line, ch: token.end };
    }

    for (var i = range.from.line; i <= range.to.line; i++) {
      var line = cm.getLine(i);
      if (line.match(/^\s*[\*|\+|\-|\d\.]\s*/)) {
        line = '  ' + line;
      }
      else {
        if (ordered) {
          line = (i - range.from.line + 1) + '. ' + line;
        }
        else {
          line = '- ' + line;
        }
      }
      cm.setLine(i, line);
    }
  }

  CodeMirror.commands.drupdownOrderedList = function (cm) {
    CodeMirror.commands.drupdownUnorderedList(cm, true);
  }

  CodeMirror.commands.drupdownQuote = function (cm) {
    var range = {};

    if (cm.somethingSelected()) {
      range.from = cm.getCursor('start');
      range.to = cm.getCursor('end');
    }
    else {
      var pos = cm.getCursor();
      var token = cm.getTokenAt(pos);
      range.from = { line: pos.line, ch: token.start };
      range.to = { line: pos.line, ch: token.end };
    }

    for (var i = range.from.line; i <= range.to.line; i++) {
      var line = cm.getLine(i);
      if (line.match(/^\s*\>\s*/)) {
        line = '  ' + line;
      }
      else {
        line = '> ' + line;
      }
      cm.setLine(i, line);
    }
  }

  CodeMirror.commands.drupdownLink = function (cm) {
    var range = {};

    if (cm.somethingSelected()) {
      range.from = cm.getCursor('start');
      range.to = cm.getCursor('end');
    }
    else {
      var pos = cm.getCursor();
      var token = cm.getTokenAt(pos);
      range.from = { line: pos.line, ch: token.start };
      range.to = { line: pos.line, ch: token.end };
    }
    var text = cm.getRange(range.from, range.to);
    if (text.match(/^\s*$/)) {
      text = 'Insert link text';
    }
    var url = 'http://www.drupal.org';
    if (Drupal.drupdown_clipboard_value) {
      url = Drupal.drupdown_clipboard_value;
    }

    cm.replaceRange("[" + text + "](" + url + " \"" + text + "\")", range.from, range.to);
  }

  Drupal.codemirror['text/x-drupdown'] = {};
  Drupal.codemirror['text/x-drupdown'].config = function(config) {
    // Map commands to keys.
    config.extraKeys = config.extraKeys || {};
    config.extraKeys.Enter = 'drupdownContinueList';
    config.extraKeys['Ctrl-H'] = 'drupdownHeadline';
    config.extraKeys['Ctrl-I'] = 'drupdownEmphasize';
    config.extraKeys['Ctrl-B'] = 'drupdownStrong';
    config.extraKeys['Ctrl-U'] = 'drupdownUnorderedList';
    config.extraKeys['Ctrl-O'] = 'drupdownOrderedList';
    config.extraKeys['Ctrl-Q'] = 'drupdownQuote';
    config.extraKeys['Ctrl-L'] = 'drupdownLink';
    config.toolbar = {
      headline: {
        label: 'Headline',
        command: 'drupdownHeadline'
      },
      emphasize: {
        type: 'group',
        children: {
          em: {
            label: '<em>I</em>',
            command: 'drupdownEmphasize'
          },
          strong: {
            label: '<strong>B</strong>',
            command: 'drupdownStrong'
          }
        }
      },
      lists: {
        type: 'group',
        children: {
          ul: {
            label: 'UL',
            command: 'drupdownUnorderedList'
          },
          ol: {
            label: 'OL',
            command: 'drupdownOrderedList'
          }
        }
      },
      blocks: {
        type: 'group',
        children: {
          quote: {
            label: 'Quote',
            command: 'drupdownQuote'
          },
          embed: {
            label: 'Link',
            command: 'drupdownLink'
          }
        }
      }
    };
  };

}());