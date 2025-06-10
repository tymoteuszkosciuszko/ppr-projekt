#!/usr/bin/perl

use strict;
use warnings;
use HTTP::Daemon;
use XML::RPC;
use IO::Handle;

# Ustawienia
my $port = 8080;
my $host = '127.0.0.1';
my $outfile = "perl-out";

# Wyjście binarne bez buforowania
$| = 1; # autoflush STDOUT
binmode STDOUT;

# Otwórz plik do zapisu binarnego
open(my $out_fh, '>', $outfile) or die "Cannot open $outfile: $!";
binmode $out_fh;

# Serwer HTTP + XML-RPC
my $server = HTTP::Daemon->new(
    LocalAddr => $host,
    LocalPort => $port,
) or die "HTTP::Daemon->new(): $!";

my $xmlrpc = XML::RPC->new();

# Flaga końca transmisji
my $eof_received = 0;

# Obsługa metody XML-RPC
sub handler {
    my ($method, @params) = @_;

    if ($method eq 'receive_byte') {
        my $byte = $params[0];          # Liczba całkowita (0-255)
        my $char = chr($byte);         # Konwersja do bajtu

        print STDOUT $char;             # Wypisz na stdout
        print $out_fh $char;           # Zapisz do pliku
        $out_fh->flush();               # Natychmiastowy zapis
    }

    return "OK";
}

# Główna pętla serwera
while (my $client = $server->accept) {
    my $connection_closed = 0;

    while (!$connection_closed && defined(my $req = $client->get_request)) {
        my $content = $xmlrpc->receive($req->content, \&handler);
        my $res = HTTP::Response->new(200);
        $res->content($content);
        $client->send_response($res);

        # Jeśli klient zakończył połączenie
        #if ($req->header('Connection') =~ /close/i) {
        #    $connection_closed = 1;
        #}
    }

    $client->close;
    $eof_received = 1;
}

# Po zakończeniu działania serwera
close($out_fh);

# Dodatkowo można zakończyć działanie po pierwszym kliencie
exit 0;
