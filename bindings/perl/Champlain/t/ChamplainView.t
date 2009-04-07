#!/usr/bin/perl

use strict;
use warnings;

use Clutter::TestHelper tests => 40;

use Champlain ':coords';


exit tests();


sub tests {
	test_go_to();
	test_generic();
	test_zoom();
	test_event();
	return 0;
}


#
# Test some default functionality.
#
sub test_generic {
	my $view = Champlain::View->new();
	isa_ok($view, 'Champlain::View');
	
	# center_on() can be tested by checking the properties latitude and longitude.
	# And even then, the values set are not the ones returned
	my $latitude = $view->get('latitude');
	my $longitude = $view->get('longitude');
	$view->center_on(48.144722, 17.112778);
	ok($view->get('latitude') != $latitude, "center_on() changed latitude");
	ok($view->get('longitude') != $longitude, "center_on() changed longitude");
	
	# set_size() can be tested by checking the properties width and height
	is($view->get('width'), 0, "original width");
	is($view->get('height'), 0, "original height");
	$view->set_size(600, 400);
	is($view->get('width'), 600, "set_size() changed width");
	is($view->get('height'), 400, "set_size() changed height");

	
	# Can't be tested but at least we check that it doesn't crash when invoked
	my $layer = Champlain::Layer->new();
	$view->add_layer($layer);
	
	
	# Change the map source (get a different map source)
	my $source_original = $view->get('map-source');
	my $source_new = Champlain::MapSource->new_osm_mapnik();
	if ($source_original->get_name eq $source_new->get_name) {
		# Same kind of map source, take another one
		$source_new = Champlain::MapSource->new_oam();
	}
	$view->set_map_source($source_new);
	is($view->get('map-source'), $source_new, "Change map source");
}


#
# Test the zoom functionality.
#
sub test_zoom {
	my $view = Champlain::View->new();
	isa_ok($view, 'Champlain::View');
	
	
	# Zoom in
	is($view->get('zoom-level'), 0, "original zoom-level");
	$view->zoom_in();
	is($view->get('zoom-level'), 1, "zoom-in once");
	$view->zoom_in();
	is($view->get('zoom-level'), 2, "zoom-in twice");
	
	# Zoom out
	$view->zoom_out();
	is($view->get('zoom-level'), 1, "zoom-out once");
	$view->zoom_out();
	is($view->get('zoom-level'), 0, "zoom-out twice");
	
	my $map_source = $view->get('map-source');
	
	# Zoom out past the min zoom level
	my $min = $map_source->get_min_zoom_level;
	$view->set_zoom_level($min);
	is($view->get('zoom-level'), $min, "zoom-out to the minimal level");

	$view->set("zoom-level", $min);
	is($view->get('zoom-level'), $min, "set('zoom-level') to the minimal level");

	$view->zoom_out();
	is($view->get('zoom-level'), $min, "zoom-out past minimal level has no effect");
	
	
	# Zoom in after the max zoom level
	my $max = $map_source->get_max_zoom_level;
	$view->set_zoom_level($max);
	is($view->get('zoom-level'), $max, "zoom-in to the maximal level");

	$view->set("zoom-level", $max);
	is($view->get('zoom-level'), $max, "set('zoom-level') to the maximal level");

	$view->zoom_in();
	is($view->get('zoom-level'), $max, "zoom-in past maximal level has no effect");
	
	# Go to the middle zoom level
	my $middle = int( ($max - $min) / 2 );
	$view->set_zoom_level($middle);
	is($view->get('zoom-level'), $middle, "set zoom to the middle level");

	$view->set("zoom-level", $middle);
	is($view->get('zoom-level'), $middle, "set('zoom-level', (max-min)/2) to the middle level");
	
	
	# Try to set directly the zoom level to a value inferior to min level
	$view->set_zoom_level($min - 1);
	is($view->get('zoom-level'), $middle, "set zoom (min - 1) has no effect");

	# NOTE: This gives a warning because -1 out of range for property `zoom-level'
	#$view->set("zoom-level", $min - 1);
	#is($view->get('zoom-level'), $middle, "set('zoom-level', min - 1) has no effect");
	
	# Try to set directly the zoom level to a valu superior to max level
	$view->set_zoom_level($max + 1);
	is($view->get('zoom-level'), $middle, "set zoom (max + 1) has no effect");

	$view->set("zoom-level", $max + 1);
	is($view->get('zoom-level'), $middle, "set('zoom-level', max + 1) has no effect");
}


#
# Test getting the coordinates from an event.
#
# This tests simulates that the user clicked at the coordinate (0, 0) (where
# Greenwich meets the Equator). In order to simulate this, the test sets the
# view to be as big as the first tile and will simulate a click in the middle of
# the tile. Because the computations are made with a projection a slight error
# threshold will be accepted.
#
sub test_event {
	my $view = Champlain::View->new();
	isa_ok($view, 'Champlain::View');
	
	my $map_source = $view->get('map-source');
	my $size = $map_source->get_tile_size;
	ok($size > 0, "Tile has a decent size");
	
	# NOTE: At the moment this works only if the view is in a stage and if
	# show_all() was called
	my $stage = Clutter::Stage->get_default();
	$stage->set_size($size, $size);
	$view->set_size($size, $size);
	$view->center_on(0, 0);
	$stage->add($view);
	$stage->show_all();
	
	# Create a fake event in the middle of the tile
	my $event = Clutter::Event->new('button_press');
	my $middle = int($size/2);
	$event->x($middle);
	$event->y($middle);
	is($event->x, $middle, "Fake event is in the middle (x)");
	is($event->y, $middle, "Fake event is in the middle (y)");

	my ($latitude, $longitude) = $view->get_coords_from_event($event);
	ok($latitude >= -2.0 && $latitude <= 2.0, "get_coords_from_event() latitude");
	ok($longitude >= -2.0 && $longitude <= 2.0, "get_coords_from_event() longitude");
}


#
# Test going to a different location with go_to().
#
sub test_go_to {
	my $view = Champlain::View->new();
	isa_ok($view, 'Champlain::View');

	# Place the view in the center
	$view->center_on(0, 0);
	is($view->get('latitude'), 0, "center_on() reset latitude");
	is($view->get('longitude'), 0, "center_on() reset longitude");
	
	
	# Go to a different place
	my ($latitude, $longitude) = (48.218611, 17.146397);
	$view->go_to($latitude, $longitude);

	# Give us a bit of time to get there  since this is an animation and it
	# requires an event loop.
	Glib::Timeout->add (100, sub {
		Clutter->main_quit();
		return FALSE;
	});
	Clutter->main();
	
	# Check if we got somewhere close to desired location (~ 1 degree)
	my ($current_latitude, $current_longitude) = $view->get('latitude', 'longitude');
	my $delta_latitude = $view->get('latitude') - $latitude;
	my $delta_longitude = $view->get('longitude') - $longitude;
	ok($delta_latitude >= -1 && $delta_latitude <= 1, "go_to() changed latitude close enough");
	ok($delta_longitude >= -1 && $delta_longitude <= 1, "go_to() changed longitude close enough");
	
	
	
	# Replace the view in the center
	$view->center_on(0, 0);
	is($view->get('latitude'), 0, "center_on() reset latitude");
	is($view->get('longitude'), 0, "center_on() reset longitude");
	
	# Go to a different place. This is too fast and can't be tested properly.
	$view->go_to($latitude, $longitude);
	$view->stop_go_to();

	is($view->get('latitude'), 0, "stop_go_to() at latitude 0");
	is($view->get('longitude'), 0, "stop_go_to() at longitude 0");
}