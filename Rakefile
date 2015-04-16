require 'rake'
require 'yaml'

TARGET = "i386-unknown-none-elf"
CC = "clang"
CFLAGS = "-ffreestanding --sysroot=src -I src/include -target #{TARGET} -c"
AS = "nasm"
ASFLAGS = "-f elf32"
OBJDIR = "objects/"
LDFLAGS = "-m elf_i386 -gc-sections -static -nostartfiles -nodefaultlibs"

SOURCE_FILES = Rake::FileList.new("src/**/*.s", "src/**/*.c")

task :default => "popcorn"

file "popcorn" => ["src/buildver.inc", :objects] do
  sh "ld #{LDFLAGS} -T src/link.ld -o popcorn #{SOURCE_FILES.pathmap("%{^src,obj}X.o")}"
  sh "rm src/buildver.inc"
end


task :objects => SOURCE_FILES.pathmap("%{^src,obj}X.o")

file "src/buildver.inc" do
  version = YAML.load_file('version.yml')
  sh "echo 'VERSION dd \"Popcorn kernel v#{version['major']}.#{version['minor']}.#{version['patch']}-#{version['additional_labels']}\", 0' > src/buildver.inc"
  sh "echo 'BUILD_DATE dd \"Build date #{DateTime.now}\",0' >> src/buildver.inc"
  # SOURCE_FILES.include("src/buildver.inc")
end


directory "obj"

rule ".o" => [->(f){f.pathmap("%{^obj,src}X.s")}, "obj"] do |srcfile|
  mkdir_p srcfile.name.pathmap("%d")
  sh "#{AS} #{ASFLAGS} -o #{srcfile.name} #{srcfile.source}"
end

rule ".o" => [->(f){f.pathmap("%{^obj,src}X.c")}, "obj"] do |srcfile|
  mkdir_p srcfile.name.pathmap("%d")
  sh "#{CC} #{CFLAGS} -o #{srcfile.name} #{srcfile.source}"
end

#todo write rule to make rnux.dsk image
directory "img"
task :qemu_disk => ["popcorn", "rnux.dsk", "img"] do
  loopfile = `losetup -f`.chomp
  if loopfile == ''
    raise "No free loopback devices available"
  else
    sh "sudo losetup #{loopfile} -o 1048576 rnux.dsk"
    sh "sudo mount #{loopfile} img"
    sh "sudo cp popcorn img/boot/"
    sh "sudo umount img"
    sh "sudo losetup -d #{loopfile}"
  end
end

task :clean do
  sh "rm -rf obj popcorn"
end
