#!/usr/bin/php

<?php
#===================================================================
# Wersja z wywolaniami zblizonymi do C
#===================================================================

# zmienne predefiniowane -------------------------------------------
$host = "127.0.0.1";
$port = 12345;

# tworzymy gniazdo -------------------------------------------------
if( ! ( $server = socket_create( AF_INET, SOCK_STREAM, SOL_TCP ) ) ){
    print "socket_create(): " 		. socket_strerror( socket_last_error( $server ) ) . "\n";
    exit( 1 );
}

# ustawiamy opcje gniazda (REUSEADDR) ------------------------------
if( ! socket_set_option($server, SOL_SOCKET, SO_REUSEADDR, 1) ) {
    print "socket_set_option(): " 	. socket_strerror(socket_last_error( $server ) ) . "\n";
    exit( 1 );
}

# mapujemy gniazdo (na port) ---------------------------------------
if( ! socket_bind( $server, $host, $port ) ){
    print "socket_bind(): " 		. socket_strerror( socket_last_error( $server ) ) . "\n";
    exit( 1 );
}

# ustawiamy gniazdo w tryb nasluchiwania ---------------------------
if( ! socket_listen( $server, 5 ) ){
    print "socket_listen(): " 		. socket_strerror( socket_last_error( $server ) ) . "\n";
    exit( 1 );
}

# obslugujemy kolejnych klientow, jak tylko sie podlacza -----------
while( $client = socket_accept( $server ) ){

    $pid = pcntl_fork();
    if ($pid == -1) {
        print "błąd pcntl_fork()\n";
        exit( 1 );
    } elseif ($pid) {
        # Proces rodzica
        socket_close($client);
    } else {
        # Proces dziecka
        socket_getpeername( $client, $addr, $port );

        # Odczytujemy PID klienta
        $client_pid = socket_read($client, 5);
        $filename = trim($client_pid) . ".txt";
        $file = fopen($filename, 'w');
        print "Adres: $addr Port: $port PID: $client_pid\n";

        while ($data = socket_read($client, 1)) {
            $hex = bin2hex($data);
            fwrite($file, "Odebrano: $data Hex: $hex\n");
            print("Odebrano: $data Hex: $hex\n");
        }

        fclose($file);
        socket_close($client);
        exit(0);
    }
}
#-------------------------------------------------------------------
socket_close( $server );
#===================================================================
?>