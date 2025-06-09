#!/usr/bin/php
<?php

$host = "127.0.0.1";
$port = 12345;
$udp_host = "127.0.0.1";
$udp_port = 54321;

$server = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
socket_set_option($server, SOL_SOCKET, SO_REUSEADDR, 1);
socket_bind($server, $host, $port);
socket_listen($server, 5);

$udp = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
socket_set_option($udp, SOL_SOCKET, SO_RCVTIMEO, ["sec" => 2, "usec" => 0]); // timeout 2 sekundy

while ($client = socket_accept($server)) {
    $pid = pcntl_fork();
    if ($pid == -1) {
        print "błąd pcntl_fork()\n";
        exit(1);
    } elseif ($pid) {
        socket_close($client);
    } else {
        socket_getpeername($client, $addr, $port);
        $client_pid = socket_read($client, 5);
        $filename = trim($client_pid) . ".txt";
        $file = fopen($filename, 'wb');
        print "Adres: $addr Port: $port PID: $client_pid\n";

        while (true) {
            $data = socket_read($client, 1);
            if ($data === false || $data === '') break;

            // fwrite($file, $data); // lokalny zapis opcjonalnie

            $ack_received = false;
            while (!$ack_received) {
                socket_sendto($udp, $data, 1, 0, $udp_host, $udp_port);

                $ack = '';
                $from = '';
                $from_port = 0;
                $bytes = @socket_recvfrom($udp, $ack, 3, 0, $from, $from_port);

                if ($bytes > 0 && $ack === 'ACK') {
                    $ack_received = true;
                } else {
                    print "[WARN] Brak ACK – ponawiam wysyłkę\n";
                    usleep(100000); // 100 ms opóźnienia
                }
            }
        }

        fclose($file);
        socket_close($client);
        exit(0);
    }
}
socket_close($server);
?>
