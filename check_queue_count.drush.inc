<?php

/**
 * Implements hook_drush_command().
 */
function check_queue_count_drush_command() {

  $items['check-queue-count'] = array(
    'description' => 'Count the queue items and return a status.',
    'aliases' => array('cqc'),
    'arguments' => array(
      'queue' => 'The name of the queue you want to get a status about.'
    )
  );

  return $items;
}

/**
 * Callback for the drush-demo-command command
 */
function drush_check_queue_count($queue_name = null) {
  $warning_count = 100;
  $error_count = 200;

  if (!empty($queue_name)) {
    $query = db_query("SELECT count(*) AS count FROM {queue} WHERE name = ':name'", array(':name' => $queue_name));
    $result = $query->execute();
  } else {
    $query = db_query("SELECT count(*) AS count FROM {queue}");
    $result = $query->execute();
  }

  $result_assoc = $query->fetchAssoc();
  $count = (int)$result_assoc['count'];

  $return = 0;
  if ($count >= $warning_count) {
    $return = 1;
  }
  if ($count >= $error_count) {
    $return = 2;
  }

  exit($return);
}
