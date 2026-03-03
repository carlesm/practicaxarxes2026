"""VPN Ethernet Switch Server entry point."""

from __future__ import annotations

import logging
import sys 

from dataclasses import dataclass
from enum import Enum
from pathlib import Path
from typing import Optional


from loguru import logger

from icecream import ic
ic.disable()

import typer
app = typer.Typer()


class UnknownMacPolicy(str, Enum):
    flood = "flood"
    discard = "discard"


@dataclass
class ServerConfig:
    port: int
    unknown_mac: UnknownMacPolicy
    stats_interval: int
    timeout: int
    credentials: Optional[Path]
    verbose: bool





@app.command()
def main(
    port: int = typer.Option(..., help="UDP port to listen on"),
    unknown_mac: UnknownMacPolicy = typer.Option(..., help="Policy for unknown destination MACs: flood or discard"),
    stats_interval: int = typer.Option(10, help="Seconds between statistics prints"),
    timeout: int = typer.Option(30, help="Seconds without keepalive before session expires"),
    credentials: Optional[Path] = typer.Option(None, help="Path to TOML credentials file"),
    verbose: bool = typer.Option(False, help="Enable debug-level logging"),
) -> None:
    """VPN Ethernet Switch Server."""

    if verbose:
        logger.remove()
        logger.add(
            sys.stderr,
            level=logging.DEBUG,
        )
        logger.debug("Verbose mode enabled, setting log level to DEBUG")
    
    config = ServerConfig(
        port=port,
        unknown_mac=unknown_mac,
        stats_interval=stats_interval,
        timeout=timeout,
        credentials=credentials,
        verbose=verbose,
    )

    logger.info(
        "Starting VPN switch on port {config.port} (unknown-mac={unknown_mac}, timeout={timeout}s, stats-interval={stats_interval}s)",
        config=config,
        unknown_mac=config.unknown_mac.value,
        timeout=config.timeout,
        stats_interval=config.stats_interval,
    )

    ic(config)
    # TODO: Implementar el servidor. Creant una instància de una classe (VpnServer, per exemple) 
    # que encapsuli tota la lògica del servidor i cridar al seu mètode run() (exemple). 
    # Això ajudarà a mantenir el codi net i organitzat.
    #       
    # server = VpnServer(config)
    # server.run()
    pass


def cli_main() -> None:
    """Entry point wrapper that converts Click's exit code 2 (usage error) to 1."""
    try:
        app()
    except SystemExit as exc:
        raise SystemExit(1 if exc.code == 2 else exc.code)


if __name__ == "__main__":
    cli_main()
