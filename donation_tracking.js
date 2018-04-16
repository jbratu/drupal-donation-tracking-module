(function ($) {

// Store our function as a property of Drupal.behaviors.
    Drupal.behaviors.myModuleSecureLink = {
        attach: function (context, settings) {
            // Find all the secure links inside context that do not have our processed
            // class.
            $('a[href^="https://"]', context)
            // Only process elements once.
                .once('secureLink')
                // Then stick some text into the link denoting it as secure.
                .append(' (Secure!)');

            document.getElementById("edit-field-doner-reference-und-0-target-id").tabIndex = 1;
            document.getElementById("edit-field-amount-und-0-value").tabIndex = 2;
            document.getElementById("edit-field-payment-type-und").tabIndex = 3;
            document.getElementById("edit-field-payment-number-und-0-value").tabIndex = 4;

            document.getElementById("edit-submit").tabIndex = 5;
            console.log('ran tab index set');
            $('#edit-field-doner-reference-und-0-target-id').focus();


        }
    };

// You could add additional behaviors here.
    Drupal.behaviors.myModuleMagic = {
        attach: function (context, settings) { },
        detach: function (context, settings) { }
    };

}(jQuery));

