<?php

function donation_tracking_mail_merge($year = null) {

  if($year == null) {
    $year = '2017';
  }

  $merge = array();

  $donations = views_get_view_result("tax_letter_mail_merge","", $year);

  foreach($donations as $donation) {

    $doner_nid = $donation->field_field_doner_reference[0]['raw']['entity']->nid;
    $amount = $donation->field_field_amount[0]['raw']['value'];

    if(isset($merge[$doner_nid])) {
      $merge[$doner_nid]['amount'] += $amount;
    } else {
      $merge[$doner_nid]['amount'] = $amount;

      if(!empty($donation->_field_data['node_field_data_field_doner_reference_nid']['entity']->field_name['und'][0]['value'])) {
        $name = $donation->_field_data['node_field_data_field_doner_reference_nid']['entity']->field_name['und'][0]['value'];
      } else {
        $name = $donation->_field_data['node_field_data_field_doner_reference_nid']['entity']->title;
      }

      $merge[$doner_nid]['name'] = $name;

      if(!isset($donation->_field_data['node_field_data_field_doner_reference_nid']['entity']->field_doner_address['und'][0]['value']))
        $address = '';
      else
        $address = $donation->_field_data['node_field_data_field_doner_reference_nid']['entity']->field_doner_address['und'][0]['value'];

      $merge[$doner_nid]['address'] = $address;

      if(!isset($donation->_field_data['node_field_data_field_doner_reference_nid']['entity']->field_doner_city_state_zip['und'][0]['value']))
        $csz = '';
      else
        $csz = $donation->_field_data['node_field_data_field_doner_reference_nid']['entity']->field_doner_city_state_zip['und'][0]['value'];

      $merge[$doner_nid]['city_state_zip'] = $csz;
      $merge[$doner_nid]['city'] = '';
      $merge[$doner_nid]['state'] = '';
      $merge[$doner_nid]['zip'] = '';
      $merge[$doner_nid]['year'] = $year;
    }

  }

  //
  //Mail merge the letters
  //

  $html = '';

  foreach($merge as $letter) {
    $html .= donation_tracking_merge_letter_into_template($letter);
  }

  $html = donation_tracking_merge_into_template($html, variable_get('dt_body_template', 'template_page.html'));

  $html = str_replace('{date}', date('F jS, Y'), $html);
  $html = str_replace('{treasurer}', variable_get('dt_treasurer', 'Your Name Here'), $html);
  $html = str_replace('{year}', $year, $html);

  $file = file_save_data($html, '', TRUE);

  $file_path = drupal_realpath($file->uri);

  if(copy($file_path,'c:/temp/letters.html'))
    drupal_set_message("Created letters.");
  else
    return "Could not create letters";

  //
  //Mail merge the envelopes
  //

  $html = '';

  foreach($merge as $letter) {
    $html .= donation_tracking_merge_envelope_into_template($letter);
  }

  $html = donation_tracking_merge_into_template($html, variable_get('dt_body_envelope_template', 'template_envelope_page.html'));

  $html = str_replace('{sender_name}',"Church Name",$html);
  $html = str_replace('{sender_address}',"Church Address",$html);
  $html = str_replace('{sender_city_state_zip}',"City, State Zip",$html);

  $file = file_save_data($html, '', TRUE);

  $file_path = drupal_realpath($file->uri);

  if(copy($file_path,'c:/temp/envelopes.html'))
    drupal_set_message("Created envelopes.");
  else
    return "Could not create envelopes";

  return "done.";

}

function donation_tracking_merge_into_template($letters_html, $file_name) {

  $file_path = drupal_get_path('module', 'donation_tracking') . '/mail_merge/' . $file_name;
  $html = file_get_contents($file_path);

  $html = str_replace('{page}', $letters_html, $html);

  return $html;
}

function donation_tracking_merge_letter_into_template($letter) {

  global $DT_LETTER_TEMPLATE;

  if(!isset($DT_LETTER_TEMPLATE)) {

    $file_name = variable_get('dt_body_template', 'template_body.html');
    $file_path = drupal_get_path('module', 'donation_tracking') . '/mail_merge/' . $file_name;
    $DT_LETTER_TEMPLATE = file_get_contents($file_path);

  }

  $html = $DT_LETTER_TEMPLATE;

  $html = str_replace('{name}',$letter['name'],$html);
  $html = str_replace('{address}',$letter['address'],$html);
  $html = str_replace('{city_state_zip}',$letter['city_state_zip'],$html);

  $amount = $letter['amount'];

  //Doesn't work on Windows
  //setlocale(LC_MONETARY, 'en_US');
  //$amount = '$' . money_format('%i', $amount);

  $amount = '$' . number_format($amount, 2,'.',',');

  $html = str_replace('{amount}',$amount,$html);

  return $html;
}

function donation_tracking_merge_envelope_into_template($letter) {

  global $DT_ENVELOPE_TEMPLATE;

  if(!isset($DT_ENVELOPE_TEMPLATE)) {

    $file_name = variable_get('dt_body_envelope_template', 'template_envelope_body.html');
    $file_path = drupal_get_path('module', 'donation_tracking') . '/mail_merge/' . $file_name;
    $DT_ENVELOPE_TEMPLATE = file_get_contents($file_path);

  }

  $html = $DT_ENVELOPE_TEMPLATE;

  $html = str_replace('{name}',$letter['name'],$html);
  $html = str_replace('{address}',$letter['address'],$html);
  $html = str_replace('{city_state_zip}',$letter['city_state_zip'],$html);

  return $html;
}