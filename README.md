# SubHTML
A simple program to substitute a part of HTML file from another file.

## Usage
All files are expected to be *.subhtml.
A file named subhtml.txt on the root directory is expected.

### On subhtml.txt
Here, you can write the path to files or folders. SubHTML will automatically fetch all files that have *.subhtml extension

### On *.subhtml files
To indicate substitution the usage of `[relative_path_to_file.subhtml]` is necessary.

#### Example:
```
[../templates/header.subhtml]

<body>
	<!-- Content Here -->
</body>

[../templates/footer.subhtml]

```