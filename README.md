# About

This command was made with the purpose to do checks on the queue size of then
Drupal queue. This can be used as a check inside Sensu or any other monitoring
check framework.

# Usage

The check is run by default on all queue's. And returns a warning if the
count is 100 or higher and a critical when 200 or higher.

You can use `-w [interger]` and `-c [integer]` to set your custom levels.

The first argument can be passed to specify a queue name that you want to check.
