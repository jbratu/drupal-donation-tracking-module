<?php


function donation_tracking_drush_command() {

  $items['donation-tracking'] = array(
    'description' => 'Send an email with the donation amount for date specified',
    'aliases' => array('edda'),
    'arguments' => array(
  		'date' => 'The type of statement (error or success).',
	  ),
  );

  $items['donation-tax-letters'] = array(
    'description' => 'Create envelopes and letters for year specified.',
    'aliases' => array('dtl'),
    'arguments' => array(
      'year' => '4 digit tax year',
    ),
  );

  return $items;
}

function drush_donation_tracking($date) {
	drupal_set_message(donation_tracking_get_deposit_total($date));
}

function drush_donation_tracking_donation_tax_letters($year) {
  include_once drupal_get_path('module', 'donation_tracking') . '/mail_merge/mail_merge.inc';
  donation_tracking_mail_merge($year);
}