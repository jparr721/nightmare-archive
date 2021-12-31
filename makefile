build:
	go build ./cmd/game

run:
	go run ./cmd/game

byte_slice:
	go run ./cmd/byte_slice -input $(input_file) -output $(output_file) -package $(package_name) -var $(var_name)