<?php
define('OTHER_ERROR', 3);
define('STATUS_OK', 0);

/**
 * Implements hook_drush_command().
 */
function check_queue_count_drush_command() {

  $items['check-queue-count'] = array(
    'description' => 'Count the queue items and return a status.',
    'aliases' => array('cqc'),
    'arguments' => array(
      'queue' => 'The name of the queue you want to get a status about.'
    ),
    'options' => array(
      'warning' => 'The warning level (--w=1000). If queue item count is equal or higher then WARNING status.',
      'critical' => 'The critical level (--c=1000). If queue item count is equal or higher then CRITICAL status.',
      'doubles' => 'Specified to enable a check on double items',
    )
  );

  return $items;
}

/**
 * Callback for the drush-queue-count command.
 *
 * @see drush_return_status().
 * @see drush_set_error().
 */
function drush_check_queue_count($queue_name = null) {
  $warning_count = (int) drush_get_option('warning', 100);
  $error_count_inital = (int) drush_get_option('critical', 200);
  $error_count = (int) drush_get_option('critical', 200);
  $doubles = drush_get_option('doubles', false) ? true : false;

  if ($warning_count > $error_count) {
    $error_count = $warning_count + 1;
  }

  $query = null;
  if (!empty($queue_name)) {
    $query = db_query("SELECT count(*) AS count FROM {queue} WHERE name = ':name'", array(':name' => $queue_name));
  } else {
    $query = db_query("SELECT count(*) AS count FROM {queue}");
  }
  if ($doubles) {
    if (!empty($queue_name)) {
      $query = db_query("SELECT count(*) AS count FROM {queue} WHERE name = ':name' GROUP BY name, data", array(':name' => $queue_name));
    } else {
      $query = db_query("SELECT count(*) AS count FROM {queue} GROUP BY name, data");
    }
  }

  if (!empty($query)) {
    $result = $query->execute();
  }

  $warning_levels = sprintf('(w: %d, ci: %d, c: %d, doubles: %s)', $warning_count, $error_count_inital, $error_count, ($doubles ? 'true' : 'false'));
  if ($result) {
    $result_assoc = $query->fetchAssoc();
    $count = (int)$result_assoc['count'];

    $return = 0;
    $return_message = 'The queue size looks OK. ' . $warning_levels;
    if ($count >= $warning_count) {
      $return = 1;
      $return_message = 'WARNING. The queue size is getting large. ' . $warning_levels;
    }
    if ($count >= $error_count) {
      $return = 2;
      $return_message = 'CRITICAL. The queue size is to big. ' . $warning_levels;
    }

    if ($return > 0) {
      drush_set_error($return, $return_message);
    } else {
      drush_log($return_message, 'ok');
    }
  } else {
    drush_set_error(OTHER_ERROR, 'There was no result returned.');
  }

}
