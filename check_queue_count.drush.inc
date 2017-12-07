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
function drush_check_queue_count($queue_name) {

  $query = db_query("select count(*) as count from {queue} WHERE name = ':name'", array(':name' => $queue_name));
  $result = $query->execute();

  print_r($query->fetchAssoc());

}
