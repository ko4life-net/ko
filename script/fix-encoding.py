# pip install python-magic python-magic-bin chardet
# if there are problems with loading library, uninstall python-magic-bin and install again.
import argparse
import csv
import datetime
import logging
import os
import re
import shutil
from collections import deque
from pathlib import Path

import chardet
import magic

#from charset_normalizer import from_path, detect

# Notes:
# Only problematic case was in src\server\LoginServer\ZipArchive.cpp, where it contained Polish and Korean characters.
# This was fixed manually by encoding it from CP949 and copying the polish chars from CP1250.

COMMON_ENCODING = ['ascii', 'utf-8', 'utf-8-sig', 'utf-16', 'utf-16-be', 'utf-16-le', 'cp949', 'cp950']

logger = logging.getLogger('fix-encoding')
logger.setLevel(logging.DEBUG)

# Default to Unix LF on all write operations
newline_feed = '\n'

class ColoredFormatter(logging.Formatter):
    COLORS = {
        'DEBUG': '\033[94m',    # Blue
        'INFO': '\033[92m',     # Green
        'WARNING': '\033[93m',  # Yellow
        'ERROR': '\033[91m',    # Red
        'CRITICAL': '\033[95m', # Magenta
        'RESET': '\033[0m'      # Reset color
    }

    def format(self, record):
        log_color = self.COLORS.get(record.levelname, self.COLORS['RESET'])
        reset_color = self.COLORS['RESET']
        message = super().format(record)
        return f"{log_color}{message}{reset_color}"

def prompt_user(message):
    response = input(f"{message} (y/n): ").strip().lower()
    if response != 'y':
        print("Operation cancelled.")
        return False

    return True

def get_bom_encoding(file_path):
    with open(file_path, 'rb') as file:
        bom = file.read(3)
        if bom.startswith(b'\xef\xbb\xbf'):
            return 'UTF-8-SIG'

        file.seek(0)
        bom = file.read(2)
        if bom == b'\xff\xfe':
            return 'UTF-16-LE'
        elif bom == b'\xfe\xff':
            return 'UTF-16-BE'
        else:
            return '<null>'

def is_text_file(file_path):
    # just to speed up for known files to this project, and also this all prevent
    # from empty text file to return as not a text file.
    known_exts = ['txt', 'c', 'cpp', 'h', 'hpp', 'py', 'ps1', 'cmd' 'md', 'yml', 'json']
    ext = Path(file_path).suffix.strip('.').lower()
    if ext in known_exts:
        return True

    try:
        mime = magic.Magic(mime=True)
        mime_type = mime.from_file(file_path)
        return mime_type.startswith('text')
    except Exception as e:
        logger.error(f"Error determining file type: {e}")
        return False

def convert_line_feed(file_path, crlf_to_lf):
    logger.info(f"Processing file: [{file_path}]")
    try:
        content = b''
        with open(file_path, 'rb') as file:
            content = file.read()

        if crlf_to_lf:
            content = content.replace(b'\r\n', b'\n')
        else:
            content = content.replace(b'\n', b'\r\n')

        with open(file_path, 'wb') as file:
            file.write(content)
    except Exception as e:
        logger.error(f"Error processing {file_path}: {e}")

def alter_line_feed(dir_path, encoding_csv_file, crlf_to_lf):
    global newline_feed
    if crlf_to_lf:
        logger.info(f"Converting from CRLF to LF line feed")
        newline_feed = '\n'
    else:
        logger.info(f"Converting from LF to CRLF line feed")
        newline_feed = '\r\n'

    # if a csv file is provided, use it instead of a directory
    if encoding_csv_file:
        if not os.path.isfile(encoding_csv_file):
            logger.error(f"File '{encoding_csv_file}' does not exist.")
            exit(1)

        with open(encoding_csv_file, mode='r') as csv_file:
            csv_reader = csv.DictReader(csv_file)
            for row in csv_reader:
                file_path = row['file']
                if not is_text_file(file_path):
                    logger.debug(f"'{file_path}' not a text file. Skipping...")
                    continue

                convert_line_feed(file_path, crlf_to_lf)
    else:
        if not os.path.isdir(dir_path):
            logger.error(f"Directory '{dir_path}' does not exist.")
            exit(1)

        root_path = os.path.abspath(dir_path)

        exclude_dirs = ['.git', '.vs', 'assets', 'db', 'vendor']
        exclude_files = []
        exclude_exts = []
        for root, dirs, files in os.walk(root_path):
            dirs[:] = [d for d in dirs if d.lower() not in exclude_dirs]
            files = [f for f in files if f not in exclude_files]
            files = [f for f in files if not any(Path(f).suffix.lower() == ext for ext in exclude_exts)]

            for file in files:
                file_path = os.path.join(root, file)
                if not is_text_file(file_path):
                    logger.debug(f"'{file_path}' not a text file. Skipping...")
                    continue

                convert_line_feed(file_path, crlf_to_lf)


def detect_encoding(dir_path):
    if not os.path.isdir(dir_path):
        logger.error(f"Directory '{dir_path}' does not exist.")
        exit(1)

    root_path = os.path.abspath(dir_path)

    logger.info("Detect encoding...")
    csv_file = "ko-encoding.csv"
    csv_manual_file = "ko-encoding-manual.csv"
    csv_invalid_file = "ko-encoding-invalid.csv"
    with open(csv_file, mode='w', newline=newline_feed) as f:
        writer = csv.writer(f)
        writer.writerow(["confidence", "encoding", "file"])

    shutil.copy2(csv_file, csv_manual_file)

    with open(csv_manual_file, mode='w', newline=newline_feed) as f:
        writer = csv.writer(f)
        writer.writerow(["confidence", "encoding", "file"])

    with open(csv_invalid_file, mode='w', newline=newline_feed) as f:
        writer = csv.writer(f)
        writer.writerow(["output", "file"])

    exclude_dirs = ['.git', '.vs', 'assets', 'db', 'vendor']
    exclude_files = [csv_file, csv_manual_file, csv_invalid_file, 'fix-encoding.py']
    exclude_exts = []
    for root, dirs, files in os.walk(root_path):
        dirs[:] = [d for d in dirs if d.lower() not in exclude_dirs]
        files = [f for f in files if f not in exclude_files]
        files = [f for f in files if not any(Path(f).suffix.lower() == ext for ext in exclude_exts)]

        for file in files:
            file_path = os.path.join(root, file)
            logger.info(f"Detecting encoding for: [{file_path}]")

            if not is_text_file(file_path):
                logger.debug(f"'{file_path}' is not a text file.")
                continue

            try:
                encoding = ''
                confidence = 0.0

                # Note that chardet doesn't indicate whether UTF-16 is LE or BE, therefore we do this step
                # since if we have some instances with UTF-16-BE, as some of the rc2 files, we want to make
                # sure they're converted to UTF-16-LE.
                bom_enc = get_bom_encoding(file_path)
                if bom_enc != '<null>':
                    encoding = bom_enc
                    confidence = 1.0
                else:
                    #result = from_path(file_path)
                    with open(file_path, 'rb') as f:
                        result = chardet.detect(f.read())
                        encoding = result['encoding']
                        confidence = result['confidence']
                        if encoding is None or confidence is None:
                            raise ValueError(result)

                    encoding_low = encoding.lower()

                    # To prevent the potential of losing characters, note that it is generally better using
                    # Microsoft's CP949, which is an extension of EUC-KR and backward compatible. It also
                    # also contain additional Hanja characters.
                    if encoding_low == 'euc-kr' or encoding_low == 'johab':
                        logger.debug(f"Mapping '{encoding}' to 'CP949' Microsoft encoding")
                        encoding = 'CP949'

                    # Same here as with euc-kr, just for Big5 where CP950 is Microsoft's extended encoding
                    # for traditional Chinese text.
                    if encoding_low == 'big5':
                        logger.debug(f"Mapping '{encoding}' to 'CP950' Microsoft encoding")
                        encoding = 'CP950'

                target_csv_file = csv_file
                if confidence < 0.90:
                    target_csv_file = csv_manual_file

                if confidence >= 0.90 and encoding.lower() not in COMMON_ENCODING:
                    logger.warning(f"High confidence but uncommon encoding detected: [{confidence},{encoding},{file_path}]")
                    target_csv_file = csv_manual_file

                with open(target_csv_file, mode='a', newline=newline_feed) as f:
                    writer = csv.writer(f)
                    writer.writerow([confidence, encoding, file_path])
            except Exception as e:
                logger.warning(f"[{file_path}] received unexpected output: [{str(e)}]")
                with open(csv_invalid_file, mode='a', newline=newline_feed) as f:
                    writer = csv.writer(f)
                    writer.writerow([str(e), file_path])


def convert_file_encoding(in_file, out_file, in_enc, out_enc):
    with open(in_file, 'r', encoding=in_enc, errors='ignore') as ifile:
        with open(out_file, mode='w', encoding=out_enc, newline=newline_feed) as ofile:
            BLOCK_SIZE = 5*1024*1024 # 5mb
            while True:
                content = ifile.read(BLOCK_SIZE)
                if not content:
                    break
                ofile.write(content)

# As of Visual Studio 2010, they're utf-16 little endian with BOM encoded to better handle unicode characters.
# Note that in python when explicitly saving using utf-16-le, it will not include the BOM (byte order marker),
# therefore we write the file with utf-16 encoding, which will default to include LE in the BOM (0xFFFE).
# More insights: https://developercommunity.visualstudio.com/t/visualstudio-v1590-resource-editor-using-utf-8-bom/384705
# Note however that we can still UTF-8 it, but this is not the default behavior of Visual Studio and it may
# introduce other problems or corrupt it eventually: https://devblogs.microsoft.com/oldnewthing/20190607-00/?p=102569
def migrate_rc_to_utf16le(input_file, output_file, input_enc):
    logger.info(f"Migrating Visual Studio resource file from '{input_enc}' to 'utf-16-le' -> [{output_file}]")
    f_in_history = deque(maxlen=10)

    with open(input_file, 'r', encoding=input_enc, errors='ignore') as f_in:
        with open(output_file, mode='w', encoding='utf-16', newline=newline_feed) as f_out:
            while True:
                pos = f_in.tell()
                line = f_in.readline()
                if not line:
                    break

                f_in_history.append((pos, line))
                f_out.write(line)
                # In some special cases, the LANG_* part may be defined as a value rather than a constant.
                # However let's assume that all use actual constants. Undefined RC files should be fixed manually.
                # Shouldn't be an issue unless one messed up their own RC file.
                if line.startswith('LANGUAGE LANG_'):
                    current_lang = line.split()[1].strip(',')
                    logger.info(f"Current language section -> {current_lang}")
                    resource_section_str = ''
                    while f_in_history:
                        prev_line = f_in_history.pop()
                        if re.match(r"// .* resources", prev_line[1]) and f_in_history.pop()[1].startswith('//////////////'):
                            resource_section_str = prev_line[1]
                            logger.debug(f"Language section start str -> {resource_section_str}")
                            break

                    # Most cases I've seen were Big5 or CP950 (Microsoft variant of extending Big5) encoded when both Korean and
                    # Chinese (Taiwan) characters are included. However since Korean character set are CP949
                    if current_lang == 'LANG_KOREAN':
                        logger.info(f"Fixing {current_lang} resources...")
                        with open(input_file, 'r', encoding='euc-kr', errors='ignore') as f_korean:
                            f_korean.seek(f_in.tell())
                            while True:
                                korean_line = f_korean.readline()
                                if not korean_line:
                                    logger.error(f"We shouldn't reach end of file here without #endif ({f_korean.tell()})")
                                    exit(1)

                                f_out.write(korean_line)
                                if korean_line == f"#endif    {resource_section_str}":
                                    f_in.seek(f_korean.tell())
                                    break

def transcode_encoding(file_path):
    if not os.path.isfile(file_path):
        logger.error(f"File '{file_path}' does not exist.")
        exit(1)

    with open(file_path, mode='r') as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            confidence = float(row['confidence'])
            in_file = row['file']
            in_enc = row['encoding'].lower()
            out_tmp_file = in_file + ".tmp"

            if in_file.endswith('.rc') or in_file.endswith('.rc2'):
                if in_enc != 'utf-16-le':
                    logger.info(f"Processing RC file: [{in_file}]")
                    os.replace(in_file, out_tmp_file)
                    migrate_rc_to_utf16le(out_tmp_file, in_file, in_enc)
                    os.remove(out_tmp_file)
                else:
                    logger.info(f"RC file already utf-16-le encoded: [{in_file}], skipping...")
                continue

            if in_enc == 'utf-8':
                continue

            logger.info(f"Processing file: [{in_file}]")
            if confidence >= 0.99:
                os.replace(in_file, out_tmp_file)
                convert_file_encoding(out_tmp_file, in_file, in_enc, 'utf-8')
                os.remove(out_tmp_file)
            else:
                logger.warning(f"Low confidence ({confidence}) for file {in_file}, skipping.")


def validate_and_init_args(args):
    if args.crlf_to_lf and args.lf_to_crlf:
        print("ERROR: --crlf-to-lf and --lf-to-crlf cannot be provided at the same time.")
        exit(1)

    verbosity_level = args.verbosity
    if verbosity_level == 0:
        verbosity = logging.ERROR
    elif verbosity_level == 1:
        verbosity = logging.INFO
    elif verbosity_level >= 2:
        verbosity = logging.DEBUG

    console_handler = logging.StreamHandler()
    file_handler = logging.FileHandler(f'fix-encoding_{datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")}.log')

    console_handler.setLevel(logging.DEBUG)
    file_handler.setLevel(logging.DEBUG)

    formatter = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    colored_formatter = ColoredFormatter('%(asctime)s - %(levelname)s - %(message)s')
    console_handler.setFormatter(colored_formatter)
    file_handler.setFormatter(formatter)

    logger.addHandler(console_handler)
    logger.addHandler(file_handler)

def main(args):
    root_dir = ''
    if args.root_dir:
        root_dir = os.path.abspath(args.root_dir)
    else:
        root_dir = os.path.abspath('.')

    if args.detect_encoding:
        detect_encoding(root_dir)

    # for dangerous operations, let's confirm with the user before proceeding
    if args.encoding_file:
        # file based
        if args.crlf_to_lf:
            if prompt_user(f"Convert CRLF to LF based on csv file '{args.encoding_file}'?"):
                alter_line_feed(root_dir, args.encoding_file, True)
        elif args.lf_to_crlf:
            if prompt_user(f"Convert LF to CRLF based on csv file '{args.encoding_file}'?"):
                alter_line_feed(root_dir, args.encoding_file, False)
    else:
        # dir based
        if args.crlf_to_lf:
            if prompt_user(f"Convert CRLF to LF in all files within '{root_dir}'?"):
                alter_line_feed(root_dir, args.encoding_file, True)
        elif args.lf_to_crlf:
            if prompt_user(f"Convert LF to CRLF in all files within '{root_dir}'?"):
                alter_line_feed(root_dir, args.encoding_file, False)

    if args.encoding_file:
        if prompt_user(f"Alter encoding based on '{args.encoding_file}'?"):
            transcode_encoding(args.encoding_file)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Script to detect and update encoding")
    parser.add_argument('-d', '--root-dir', type=str, required=False,
                        help='Path to the target root directory')
    parser.add_argument('-e', '--detect-encoding', action='store_true', required=False,
                        help='Detect encoding and generate csv files with matches and confidence.')
    parser.add_argument('-f', '--encoding-file', type=str, required=False,
                        help='Path to the CSV file for encoding')
    parser.add_argument('-l', '--crlf-to-lf', action='store_true', required=False,
                        help='Convert the line feed from Windows CRLF to Unix LF.')
    parser.add_argument('-c', '--lf-to-crlf', action='store_true', required=False,
                        help='Convert the line feed from Unix LF to Windows CRLF.')
    parser.add_argument('-v', '--verbosity', action='count', default=2, required=False,
                        help='Increase verbosity level. Use -v for INFO, -vv for DEBUG. Defaults to DEBUG.')

    args = parser.parse_args()
    validate_and_init_args(args)
    main(args)
