#!/usr/bin/perl

use warnings;
use strict;
use HTTP::Daemon;
use HTTP::Response;
use XML::RPC;
use MIME::Base64;

#=======================================================================
sub handler {
    my ( $method, @params ) = @_;

    if ( $method eq 'send_data' ) {
        my $b64_data = $params[0];

        # Dekodowanie base64
        my $binary = decode_base64($b64_data);

        # Konwersja binarnych danych do hex
        my $hex = unpack("H*", $binary);

        # Wypisanie hex na STDOUT
        print "$hex\n";

        # Zwrócenie potwierdzenia
        return "OK: " . length($binary) . " bajtów odebrano i przekonwertowano.";
    }

    return "Unknown method: $method";
}
#=======================================================================
my $port 	= 8000;
my $host 	= '127.0.0.1';

# HTTP server ----------------------------------------------------------
my $server = HTTP::Daemon->new( 
	LocalAddr => $host,
	LocalPort => $port,
) || die "HTTP::Daemon->new(): $!\n";

# XML-RPC broker -------------------------------------------------------
my $xmlrpc 	= XML::RPC->new();

# Process --------------------------------------------------------------
print "[INFO] XML-RPC serwer Perl działa na http://$host:$port/\n";

while( my $client = $server->accept ){
	while( my $req = $client->get_request ){
		my $msg = $xmlrpc->receive( $req->content, \&handler );
		my $res = HTTP::Response->new( 200 );
		$res->content( $msg );
		
		$client->send_response( $res );
	}
	$client->close;
}
#=======================================================================
exit( 0 );
